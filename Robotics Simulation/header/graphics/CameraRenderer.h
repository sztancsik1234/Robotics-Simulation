#pragma once
#include "graphics/IViewport.h"
#include "graphics/IRenderer.h"
#include "util/ILogger.h"

class CameraRenderer
{
public:
    CameraRenderer(IViewport& camera, IDrawableRenderer& renderer, ILogger* logger = nullptr)
        : Viewport(camera), Renderer(renderer), Logger(logger) {}

    void DrawCircle(Vector2 worldCenter, float worldRadius);
    void DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize);
    void DrawRectangle(Vector2 worldP1, Vector2 worldP2);
    void DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor = { 0.f, 0.f });

private:
    IViewport& Viewport;
    IDrawableRenderer& Renderer;
    ILogger* Logger; // optional
};