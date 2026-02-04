#pragma once
#include "core/Transform.h"
#include "graphics/IRenderer.h"

struct ICameraRenderer
{
    virtual ~ICameraRenderer() = default;

    virtual void DrawCircle(Vector2 worldCenter, float worldRadius) = 0;
    virtual void DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize) = 0;
    virtual void DrawRectangle(Vector2 worldP1, Vector2 worldP2) = 0;
    virtual void DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor = { 0.f, 0.f }) = 0;
};
