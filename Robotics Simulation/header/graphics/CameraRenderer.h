#pragma once
#include "graphics/Viewport.h"
#include "graphics/IRenderer.h"
#include "util/ILogger.h"
#include "graphics/ICameraRenderer.h"

class CameraRenderer final : public ICameraRenderer
{
public:
    CameraRenderer(Viewport camera, IDrawableRenderer& renderer, ILogger* logger = nullptr)
        : Viewport(camera), Renderer(renderer), Logger(logger) {}

    void DrawCircle(Vector2 worldCenter, float worldRadius) override;
    void DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize) override;
    void DrawRectangle(Vector2 worldP1, Vector2 worldP2) override;
    void DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor = { 0.f, 0.f }) override;

	const Viewport& GetViewport() const { return Viewport; }

private:
    Viewport Viewport;
    IDrawableRenderer& Renderer;
    ILogger* Logger; // optional
};