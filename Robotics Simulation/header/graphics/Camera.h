/*
THIS FILE IS DEPRECATED AND IS NOT USED ANYMORE.
*/

#pragma once
#include "core/Transform.h"
#include "graphics/IRenderer.h"
#include "util/ILogger.h"
#include "graphics/IViewport.h"

// Camera: converts world-space to screen-space and delegates draws to IDrawableRenderer
class Camera : public IViewport
{
public:
    /// <summary>
    /// Constructs a Camera configured with a renderer, logger, and view parameters.
    /// </summary>
    /// <param name="renderer">Reference to the IDrawableRenderer used by the camera to perform drawing and rendering operations.</param>
    /// <param name="logger">Reference to the ILogger used for logging diagnostic and runtime messages.</param>
    /// <param name="viewCenterWorld">World-space coordinates (x, y) specifying the center of the camera's view.</param>
    /// <param name="screenSizePixels">Size of the screen in pixels (width, height) used for mapping between world and screen coordinates.</param>
    /// <param name="viewSizeWorld">Size of the view in world units (width, height) that determines the visible area.</param>
    Camera(IDrawableRenderer& renderer, 
        ILogger& logger,
        Vector2 viewCenterWorld,
        Vector2 screenSizePixels,
        Vector2 viewSizeWorld);

    Camera() = delete;

    // Camera configuration
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

    // Draw wrappers (world-space in, pixel-space out)
    void DrawCircle(Vector2 worldCenter, float worldRadius);
    void DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize);
    void DrawRectangle(Vector2 worldP1, Vector2 worldP2);
    void DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor = { 0.f, 0.f });

private:
    IDrawableRenderer& Renderer;
    ILogger& Logger;

    // Screen and view configuration
    Vector2 ScreenResolution{ 800.f, 600.f }; // pixels
    Vector2 viewportCenter{ 0.f, 0.f };           // world-units (center)
    Vector2 viewportSize{ 10.f, 10.f };           // world-units (width,height)
};