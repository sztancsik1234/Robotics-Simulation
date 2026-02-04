#pragma once
#include "core/Transform.h"
#include "graphics/IViewport.h" 


class Viewport final : public IViewport
{
public:
    Viewport(Vector2 viewCenterWorld,
        Vector2 screenSizePixels,
        Vector2 viewSizeWorld);

    Viewport() = delete;

    // Viewport configuration
    void SetViewCenter(Vector2 center);
    void SetViewSize(Vector2 size);
    void SetScreenSizePixels(Vector2 sizePixels);

    Vector2 GetViewCenter() const override;
    Vector2 GetViewSize() const override;
    Vector2 GetScreenResolution() const override;

    // Coordinate conversions
    Vector2 PixelToWorldPos(Vector2 pixelCoords) const override;
    Vector2 WorldToPixelPos(Vector2 worldPos) const override;
    Transform ToScreenSpace(const Transform& world) const override;
private:
    Vector2 ScreenResolution { 800.f, 600.f };     // pixels
    Vector2 viewportCenter { 0.f, 0.f };           // world-units (center)
    Vector2 viewportSize { 10.f, 10.f };           // world-units (width,height)
};

