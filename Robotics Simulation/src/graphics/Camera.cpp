#include "graphics/Camera.h"
#include <algorithm>
#include <cmath>

#define WORLDYUP true

// Internal-only helpers with minimal overhead

struct PixelRect
{
	float x{};
	float y{};
	float width{};
	float height{};
};

struct WorldViewPort
{
	Vector2 pos{};
	Vector2 size{};
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
	                            Vector2 screenSizePixels,
	                            bool snapToPixel) noexcept
{
	// Compute top-left of the view in world space
	const float viewLeft = viewCenter.x - (viewSize.x * 0.5f);
	const float viewTop  = viewCenter.y - (viewSize.y * 0.5f);

	// Scale factors
	const Scale s = ComputeScale(viewSize, screenSizePixels);

	// Convert to pixels directly (avoid intermediate normalized vector)
	float px = (worldPos.x - viewLeft) * s.x;
	float py = (worldPos.y - viewTop)  * s.y;

	// Flip Y if world is Y-up
	if (WORLDYUP)
	{
		py = screenSizePixels.y - py;
	}

	if (snapToPixel)
	{
		px = std::round(px);
		py = std::round(py);
	}

	return { px, py };
}

inline Vector2 WorldSizeToPixelSize(Vector2 objectSize,
	                                Vector2 viewSize,
	                                Vector2 screenSizePixels) noexcept
{
	const Scale s{ screenSizePixels.x / viewSize.x, screenSizePixels.y / viewSize.y };

	Vector2 px{ objectSize.x * s.x, objectSize.y * s.y };
	px.x = (px.x > 0.f) ? std::max(1.f, std::round(px.x)) : std::round(px.x);
	px.y = (px.y > 0.f) ? std::max(1.f, std::round(px.y)) : std::round(px.y);
	return px;
}

inline Transform WorldToScreenTransform(const Transform& worldTransform,
	                                    Vector2 viewCenter,
	                                    Vector2 viewSize,
	                                    Vector2 screenSizePixels,
	                                    bool snapToPixel = true) noexcept
{
	Transform screen{};
	screen.rotation = worldTransform.rotation;
	screen.position = WorldPosToScreen(worldTransform.position, viewCenter, viewSize, screenSizePixels, snapToPixel);
	screen.size     = WorldSizeToPixelSize(worldTransform.size, viewSize, screenSizePixels);
	return screen;
}

inline bool IsRectOnScreen(const PixelRect& r, Vector2 screenSizePixels) noexcept
{
	if (r.x + r.width  < 0)                return false;
	if (r.y + r.height < 0)                return false;
	if (r.x > screenSizePixels.x)          return false;
	if (r.y > screenSizePixels.y)          return false;
	return true;
}


Camera::Camera(IDrawableRenderer& renderer, ILogger& logger,
	Vector2 viewCenterWorld,
	Vector2 screenSizePixels,
	Vector2 viewSizeWorld)
	: Renderer(renderer),
	  Logger(logger),
	  ScreenSizePixels(screenSizePixels),
	  ViewCenter(viewCenterWorld),
	  ViewSize(viewSizeWorld)
{}

void Camera::SetViewCenter(Vector2 center) { ViewCenter = center; }
void Camera::SetViewSize(Vector2 size) { ViewSize = size; }
void Camera::SetScreenSizePixels(Vector2 sizePixels) { ScreenSizePixels = sizePixels; }

Vector2 Camera::GetViewCenter() const { return ViewCenter; }
Vector2 Camera::GetViewSize() const { return ViewSize; }
Vector2 Camera::GetScreenResolution() const { return ScreenSizePixels; }

Vector2 Camera::PixelToWorldPos(Vector2 pixelCoords, bool snapToPixel) const
{
	float px = pixelCoords.x;
	float py = pixelCoords.y;

	if (snapToPixel)
	{
		px = std::round(px);
		py = std::round(py);
	}

	// Undo Y flip applied in WorldPosToScreen
	if (WORLDYUP)
	{
		py = ScreenSizePixels.y - py;
	}

	// Compute top-left of the view in world space
	const float viewLeft = ViewCenter.x - (ViewSize.x * 0.5f);
	const float viewTop = ViewCenter.y - (ViewSize.y * 0.5f);

	// Convert back to world coordinates
	const Scale s = ComputeScale(ViewSize, ScreenSizePixels);
	const float wx = (px / s.x) + viewLeft;
	const float wy = (py / s.y) + viewTop;

	return { wx, wy };
}

void Camera::DrawCircle(Vector2 worldCenter, float worldRadius)
{
	const Vector2 centerPx = WorldPosToScreen(worldCenter, ViewCenter, ViewSize, ScreenSizePixels, true);

	const Scale s = { ScreenSizePixels.x / ViewSize.x, ScreenSizePixels.y / ViewSize.y };
	const float scaleAvg = (s.x + s.y) * 0.5f;
	const float radiusPx = std::max(1.f, std::round(worldRadius * scaleAvg));

	Renderer.DrawCircle(centerPx, radiusPx);
}

void Camera::DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize)
{
	const Vector2 topLeftPx = WorldPosToScreen(worldTopLeft, ViewCenter, ViewSize, ScreenSizePixels, true);
	const Vector2 sizePx    = WorldSizeToPixelSize(worldSize, ViewSize, ScreenSizePixels);
	Renderer.DrawRectangle(topLeftPx, sizePx.x, sizePx.y);
}

void Camera::DrawRectangle(Vector2 worldP1, Vector2 worldP2)
{
	const Vector2 p1 = WorldPosToScreen(worldP1, ViewCenter, ViewSize, ScreenSizePixels, true);
	const Vector2 p2 = WorldPosToScreen(worldP2, ViewCenter, ViewSize, ScreenSizePixels, true);
	Renderer.DrawRectangle(p1, p2);
}

void Camera::DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor)
{
	const Transform screen = WorldToScreenTransform(worldTransform, ViewCenter, ViewSize, ScreenSizePixels, true);
#ifdef _DEBUG
	if (textureId == 0)
	{
		Logger.Log("[Camera] TextureId is 0, sprite will not be drawn!", LogLevel::WARNING);
		return;
	}
#endif
	Renderer.DrawSprite(screen, textureId, spriteAnchor);
}