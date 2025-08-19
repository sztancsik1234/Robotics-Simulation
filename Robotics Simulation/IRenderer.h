#pragma once
#include "Vector2.h"
#include "Color.inl"

struct IRendererBase { virtual ~IRendererBase() = default; };

/// <summary>
/// Defines an interface for renderer lifecycle management, including window- and other initialization and shutdown operations.
/// </summary>
struct IRendererLifecycle : IRendererBase {
    virtual void Initialize() = 0;
    virtual void Shutdown() = 0;
};

/// <summary>
/// Interface for managing the lifecycle of one frame.
/// </summary>
struct IFrameRenderer : IRendererBase {
    virtual void Clear(Color color) = 0;
    virtual void DisplayFrame() = 0;
};

/// <summary>
/// Interface for rendering drawable shapes and sprites. Stores its own reference to textures and sprites if needed.
/// </summary>
struct IDrawableRenderer : IRendererBase {
    virtual void DrawLine(Vector2 start, Vector2 end) = 0;
    virtual void DrawCircle(Vector2 center, float radius) = 0;
    virtual void DrawRectangle(Vector2 topLeft, float width, float height) = 0;
    virtual void DrawRectangle(Vector2 p1, Vector2 p2) = 0;
    virtual void DrawSprite(Vector2 position, float width, float height, const char* texturePath) = 0;
};

// Convenient full interface
struct IRenderer :
    IRendererLifecycle,
    IFrameRenderer,
    IDrawableRenderer
{};