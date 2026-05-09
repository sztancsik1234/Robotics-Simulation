#pragma once
#include "core/Transform.h"
#include "graphics/IRenderer.h"
#include "core/Vector2.h"

struct ICameraRenderer
{
	virtual ~ICameraRenderer() = default;

	virtual void DrawCircle(Vector2 worldCenter, float worldRadius) = 0;
	virtual void DrawRectangleTopLeft(Vector2 worldTopLeft, Vector2 worldSize) = 0;
	virtual void DrawRectangle(Vector2 worldP1, Vector2 worldP2) = 0;
	/// <summary>
	/// Draws a sprite on the screen
	/// </summary>
	/// <param name="worldTransform">The world transform of the sprite</param>
	/// <param name="textureId">The ID of the texture to use</param>
	/// <param name="spriteAnchor">The anchor point of the sprite. It is a normalized vector2, where (0, 0) is the top-left corner and (1, 1) is the bottom-right corner.</param>
	virtual void DrawSprite(const Transform& worldTransform, TextureId textureId, const Vector2 spriteAnchor = { 0.f, 0.f }) = 0;
};
