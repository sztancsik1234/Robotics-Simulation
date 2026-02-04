#include "graphics/CameraRenderer.h"
#include <algorithm>
#include <cmath>

namespace {
    inline float AverageScale(Vector2 viewSize, Vector2 screenSizePixels)
    {
        const float sx = screenSizePixels.x / viewSize.x;
        const float sy = screenSizePixels.y / viewSize.y;
        return (sx + sy) * 0.5f;
    }
}

// TODO: make a version with pixel radius and with world radius
void CameraRenderer::DrawCircle(Vector2 worldCenter, float pixelradius)
{
    float worldRadius = 0;  // alternative passable value to the function
    const Vector2 centerPx = Viewport.WorldToPixelPos(worldCenter);
    const float scaleAvg = AverageScale(Viewport.GetViewSize(), Viewport.GetScreenResolution());
    const float radiusPx = std::max(1.f, std::round(worldRadius * scaleAvg));
    Renderer.DrawCircle(centerPx, pixelradius);

    Logger->Log(std::format("[CameraRenderer] DrawCircle pixel({}, {})",
        centerPx.x,
        centerPx.y
		), LogLevel::TRACE);
}

void CameraRenderer::DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize)
{
    const Vector2 topLeftPx = Viewport.WorldToPixelPos(worldTopLeft);

    const Vector2 viewSize = Viewport.GetViewSize();
    const Vector2 screen = Viewport.GetScreenResolution();
    const Vector2 sizePx{
        std::max(1.f, std::round(worldSize.x * (screen.x / viewSize.x))),
        std::max(1.f, std::round(worldSize.y * (screen.y / viewSize.y)))
    };
    Renderer.DrawRectangle(topLeftPx, sizePx.x, sizePx.y);
}

void CameraRenderer::DrawRectangle(Vector2 worldP1, Vector2 worldP2)
{
    const Vector2 p1 = Viewport.WorldToPixelPos(worldP1);
    const Vector2 p2 = Viewport.WorldToPixelPos(worldP2);
    Renderer.DrawRectangle(p1, p2);
}

void CameraRenderer::DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor)
{
#ifdef _DEBUG
    if (Logger && textureId == 0)
    {
        Logger->Log("[CameraRenderer] TextureId is 0, sprite will not be drawn!", LogLevel::WARNING);
        return;
    }
#endif
    const Transform screen = Viewport.ToScreenSpace(worldTransform);    
	Renderer.DrawSprite(screen, textureId, spriteAnchor);   // Size is y=0 for some reason
}