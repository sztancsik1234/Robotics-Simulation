#pragma once
#include "Vector2.h"

struct IRendererBase { virtual ~IRendererBase() = default; };

struct IRendererLifecycle : IRendererBase {
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
};

struct IFrameRenderer : IRendererBase {
    virtual void Clear() = 0;
    virtual void Render() = 0;
};

struct IPrimitiveRenderer : IRendererBase {
    virtual void DrawLine(Vector2 start, Vector2 end) = 0;
    virtual void DrawCircle(Vector2 center, float radius) = 0;
    virtual void DrawRectangle(Vector2 topLeft, float width, float height) = 0;
    virtual void DrawRectangle(Vector2 p1, Vector2 p2) = 0;
};

struct ISpriteRenderer : IRendererBase {
    virtual void DrawSprite(Vector2 position, float width, float height, const char* texturePath) = 0;
};

// Convenient full interface
struct IRenderer :
    IRendererLifecycle,
    IFrameRenderer,
    IPrimitiveRenderer,
    ISpriteRenderer
{};