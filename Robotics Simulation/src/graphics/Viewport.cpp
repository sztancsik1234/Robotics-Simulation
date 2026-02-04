#include "graphics/Viewport.h"
#include <algorithm>
#include <cmath>

#define WORLDYUP true
#define SNAPTOPIXEL_DEFAULT true

// Internal-only helpers with minimal overhead

struct PixelRect
{
	float x {};
	float y {};
	float width {};
	float height {};
};

struct WorldViewPort
{
	Vector2 pos {};
	Vector2 size {};
};

struct Scale
{
	float x;
	float y;
};

inline Scale ComputeScale(Vector2 viewSize, Vector2 screenSizePixels) noexcept
{
	return { screenSizePixels.x / viewSize.x, screenSizePixels.y / viewSize.y };
}

inline Vector2 WorldPosToScreen(Vector2 worldPos,
	Vector2 viewCenter,
	Vector2 viewSize,
	Vector2 screenSizePixels) noexcept
{
	// Translate world position to camera-centered coordinates
	Vector2 p = worldPos - viewCenter;

	Scale zoom = ComputeScale(viewSize, screenSizePixels);
	p.x *= zoom.x;
	p.y *= zoom.y;

	if constexpr (WORLDYUP) p.y = -p.y; // flip Y axis

	// Translate to screen space (origin at top-left)
	p.x += screenSizePixels.x * 0.5f;
	p.y += screenSizePixels.y * 0.5f;

	if constexpr (SNAPTOPIXEL_DEFAULT)
	{
		p.x = std::round(p.x);
		p.y = std::round(p.y);
	}

	return p;
}

inline Vector2 WorldSizeToPixelSize(Vector2 objectSize,
	Vector2 viewSize,
	Vector2 screenSizePixels) noexcept
{
	const Scale s { screenSizePixels.x / viewSize.x, screenSizePixels.y / viewSize.y };

	Vector2 px { objectSize.x * s.x, objectSize.y * s.y };
	px.x = (px.x > 0.f) ? std::max(1.f, std::round(px.x)) : std::round(px.x);
	px.y = (px.y > 0.f) ? std::max(1.f, std::round(px.y)) : std::round(px.y);
	return px;
}

inline Transform WorldToScreenTransform(const Transform& worldTransform,
	Vector2 viewCenter,
	Vector2 viewSize,
	Vector2 screenSizePixels) noexcept
{
	Transform screen {};
	screen.rotation = worldTransform.rotation; // TODO: object's rotation is opposite of camera rotation
	screen.position = WorldPosToScreen(worldTransform.position, viewCenter, viewSize, screenSizePixels);
	screen.size = WorldSizeToPixelSize(worldTransform.size, viewSize, screenSizePixels);
	return screen;
}

inline bool IsRectOnScreen(const PixelRect& r, Vector2 screenSizePixels) noexcept
{
	if (r.x + r.width < 0 ||
		r.y + r.height < 0 ||
		r.x > screenSizePixels.x ||
		r.y > screenSizePixels.y)
	{
		return false;
	}
	return true;
}


Viewport::Viewport(Vector2 viewCenterWorld,
	Vector2 screenSizePixels,
	Vector2 viewSizeWorld)
	: ScreenResolution(screenSizePixels),
	viewportCenter(viewCenterWorld),
	viewportSize(viewSizeWorld)
{

}

void Viewport::SetViewCenter(Vector2 center) { viewportCenter = center; }
void Viewport::SetViewSize(Vector2 size) { viewportSize = size; }
void Viewport::SetScreenSizePixels(Vector2 sizePixels) { ScreenResolution = sizePixels; }

Vector2 Viewport::GetViewCenter() const { return viewportCenter; }
Vector2 Viewport::GetViewSize() const { return viewportSize; }
Vector2 Viewport::GetScreenResolution() const { return ScreenResolution; }

Vector2 Viewport::PixelToWorldPos(Vector2 pixelCoords) const
{
	Vector2 p = pixelCoords;

	if constexpr (SNAPTOPIXEL_DEFAULT)
	{
		p.x = std::round(p.x);
		p.y = std::round(p.y);
	}

	// Undo Y flip applied in WorldPosToScreen
	if constexpr (WORLDYUP) p.y = ScreenResolution.y - p.y;

	// Compute top-left of the view in world space
	const Vector2 viewTopLeft = { 
		viewportCenter.x - (viewportSize.x * 0.5f),
		viewportCenter.y - (viewportSize.y * 0.5f)
	};

	// Convert back to world coordinates
	const Scale s = ComputeScale(viewportSize, ScreenResolution);
	const Vector2 w = { 
		(p.x / s.x) + viewTopLeft.x,
		(p.y / s.y) + viewTopLeft.y
	};

	return w;
}

Vector2 Viewport::WorldToPixelPos(Vector2 worldPos) const
{
	return WorldPosToScreen(worldPos, viewportCenter, viewportSize, ScreenResolution);
}

Transform Viewport::ToScreenSpace(const Transform& worldTransform) const
{
	return WorldToScreenTransform(worldTransform, viewportCenter, viewportSize, ScreenResolution);
}
