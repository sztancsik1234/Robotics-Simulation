#pragma once
#include "graphics/ICameraRenderer.h"

class MockCameraRenderer : public ICameraRenderer
{
public:
    bool drawCircleCalled = false;
    bool drawRectangleTopLeftCalled = false;
    bool drawRectangleCalled = false;
    bool drawSpriteCalled = false;

    Vector2 lastWorldCenter { 0.f, 0.f };
    float lastWorldRadius = 0.0f;

    Vector2 lastWorldTopLeft { 0.f, 0.f };
    Vector2 lastWorldSize { 0.f, 0.f };

    Vector2 lastWorldP1 { 0.f, 0.f };
    Vector2 lastWorldP2 { 0.f, 0.f };

    Transform lastWorldTransform {};
    TextureId lastTextureId = 0;
    Vector2 lastSpriteAnchor { 0.f, 0.f };

    void DrawCircle(Vector2 worldCenter, float worldRadius) override;
    void DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize) override;
    void DrawRectangle(Vector2 worldP1, Vector2 worldP2) override;
    void DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor = { 0.f, 0.f }) override;
};