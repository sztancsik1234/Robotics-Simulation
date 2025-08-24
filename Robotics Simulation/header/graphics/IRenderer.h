#pragma once
#include "core/Vector2.h"
#include "core/Color.inl"
#include <stdexcept>

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
struct IPrimitiveRenderer : IRendererBase {
    virtual void DrawLine(Vector2 start, Vector2 end) = 0;
    virtual void DrawCircle(Vector2 center, float radius) = 0;
    virtual void DrawRectangle(Vector2 topLeft, float width, float height) = 0;
    virtual void DrawRectangle(Vector2 p1, Vector2 p2) = 0;
};

struct ISpriteRenderer : IRendererBase
{
    // not sure about the return type here
    virtual unsigned int LoadTexture(const char* filePath) = 0;
    virtual unsigned int LoadTexture() = 0;
    virtual void DrawSprite(Vector2 position, unsigned int textureId, Vector2 size, const Vector2 SpriteAncor = { 0.f,0.f }) = 0;
	virtual void UnloadTexture(unsigned int textureId) = 0;
};

struct IDrawableRenderer :
    IPrimitiveRenderer,
    ISpriteRenderer
{};

// Convenient full interface
struct IRenderer :
    IRendererLifecycle,
    IFrameRenderer,
    IDrawableRenderer
{};

class TextureLoadException : public std::runtime_error {
    public:
    explicit TextureLoadException(const std::string& message)
        : std::runtime_error(message) {}
};