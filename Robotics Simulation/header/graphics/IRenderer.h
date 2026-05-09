#pragma once
#include "core/Color.inl"
#include "core/Transform.h"
#include "core/Vector2.h"
#include <stdexcept>
#include <string>

using TextureId = unsigned int;

struct IRendererBase { virtual ~IRendererBase() = default; };

/// <summary>
/// Defines an interface for renderer lifecycle management, including window- and other initialization and shutdown operations.
/// </summary>
struct IRendererLifecycle : IRendererBase {
	/// <summary>
	/// Called before every other rendering calls. Creates window and render context
	/// </summary>
	virtual void Initialize(Vector2 screenSize) = 0;

	/// <summary>
	/// Called before destruction of the class. Cleans up associated resources.
	/// </summary>
	virtual void Shutdown() = 0;

	virtual bool IsInitialized() const = 0;
};

/// <summary>
/// Interface for managing the lifecycle of one frame.
/// </summary>
struct IFrameRenderer : IRendererBase {

	/// <summary>
	/// Clears the frame
	/// </summary>
	/// <param name="color">Color to fill the new frame with.</param>
	virtual void Clear(Color color) = 0;

	/// <summary>
	/// Called at the end of the frame, displaying everithing rendered
	/// </summary>
	virtual void DisplayFrame() = 0;
};

/// <summary>
/// Interface for rendering drawable shapes and sprites. Stores its own reference to textures and sprites if needed.
/// </summary>
struct IPrimitiveRenderer : IRendererBase {

	/// <summary>
	/// Draws a line between two points. The looks is up to the implementation.
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	virtual void DrawLine(Vector2 start, Vector2 end) = 0;

	/// <summary>
	/// Draws a circle around the specified point
	/// </summary>
	/// <param name="center">The center of the circle in pixel coordinates</param>
	/// <param name="radius">The radius of the circle in pixels.</param>
	virtual void DrawCircle(Vector2 center, float radius) = 0;

	/// <summary>
	/// Draws a rectangle specified by wo points. Looks are dependent on implementation.
	/// </summary>
	/// <param name="topLeft"></param>
	/// <param name="width"></param>
	/// <param name="height"></param>
	virtual void DrawRectangle(Vector2 p1, Vector2 p2) = 0;

	/// <summary>
	/// Draws a rectangle with a given width and height at the specified point.
	/// </summary>
	/// <param name="topLeft">Pixelcoordinates of the ancor, the top left corner</param>
	/// <param name="width">width in pixels</param>
	/// <param name="height">height in pixels</param>
	virtual void DrawRectangle(Vector2 topLeft, float width, float height) = 0;
};

struct ISpriteRenderer : IRendererBase
{
	/// <summary>
	/// load a texture from a file into memory
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns>A TextureId used to identify the texture in subsequent calls</returns>
	virtual TextureId LoadTexture(const std::string filePath) = 0;

	/// <summary>
	/// Load a default texture
	/// </summary>
	/// <returns>A TextureId used to identify the texture in subsequent calls</returns>
	virtual TextureId LoadTexture() = 0;

	/// <summary>
	/// Draws a sprite at a specified position with a given texture, scale, and anchor point.
	/// </summary>
	/// <param name="position">The position on the screen where the sprite will be drawn.</param>
	/// <param name="textureId">The identifier of the texture to use for the sprite.</param>
	/// <param name="scale">The size of the drawn sprite in pixels.</param>
	/// <param name="SpriteAnchor">The anchor point for the sprite's origin, specified as a normalized vector. {0,0} for top left corner, {1,1} for bottom right</param>
	virtual void DrawSprite(const Transform& transform, TextureId textureId, const Vector2 SpriteAnchor = { 0.f,0.f }) = 0;

	/// <summary>
	/// Unloads a texture safely
	/// </summary>
	/// <param name="textureId">The TextureId of the texture to be unloaded</param>
	virtual void UnloadTexture(TextureId textureId) = 0;
};

struct IDrawableRenderer :
	IPrimitiveRenderer,
	ISpriteRenderer
{
};

// Convenient full interface
struct IRenderer :
	IRendererLifecycle,
	IFrameRenderer,
	IDrawableRenderer
{
};

class TextureLoadException : public std::runtime_error {
public:
	using runtime_error::runtime_error;
};