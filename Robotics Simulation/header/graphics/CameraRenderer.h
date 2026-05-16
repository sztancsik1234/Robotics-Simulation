#pragma once
#include "graphics/ICameraRenderer.h"
#include "graphics/IRenderer.h"
#include "graphics/Viewport.h"
#include "util/ILogger.h"
#include "core/Transform.h"
#include "core/Vector2.h"

class CameraRenderer final : public ICameraRenderer
{
public:
	CameraRenderer(Viewport const& camera, IDrawableRenderer& renderer, ILogger* logger = nullptr)
		: Viewport(camera), Renderer(renderer), Logger(logger)
	{}

	void DrawCircle(Vector2 worldCenter, float worldRadius) override;
	void DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize) override;
	void DrawRectangle(Vector2 worldP1, Vector2 worldP2) override;
	
	/// <summary>
	/// Draws a sprite on the screen
	/// </summary>
	/// <param name="worldTransform">The world transform of the sprite</param>
	/// <param name="textureId">The ID of the texture to use</param>
	/// <param name="spriteAnchor">The anchor point of the sprite. It is a normalized vector2, where (0, 0) is the top-left corner and (1, 1) is the bottom-right corner.</param>
	void DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor = { 0.f, 0.f }) override;

	const Viewport& GetViewport() const { return Viewport; }

private:
	Viewport Viewport;
	IDrawableRenderer& Renderer;
	ILogger* Logger; // optional
};