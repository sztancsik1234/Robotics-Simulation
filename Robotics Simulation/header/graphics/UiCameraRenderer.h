#pragma once
#include "ICameraRenderer.h"
#include "IdentityViewport.h"
#include <util/ILogger.h>
class UiCameraRenderer final : public ICameraRenderer
{
public:
	UiCameraRenderer(IdentityViewport const& camera, IDrawableRenderer& renderer, ILogger* logger = nullptr)
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

	const IdentityViewport& GetViewport() const { return Viewport; }

private:
	IdentityViewport Viewport;
	IDrawableRenderer& Renderer;
	ILogger* Logger; // optional
};

