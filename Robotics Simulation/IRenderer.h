#pragma once
#include "Coordinates.h"

struct IRenderer
{
	virtual ~IRenderer() = default;
	// Initialize the renderer
	virtual void Initialize() = 0;
	// clear the screen
	virtual void Clear() = 0;
	// draw functions for various primitives
	virtual void DrawLine(Vector2 p) = 0;
	virtual void DrawCircle(Vector2 p, float radius) = 0;
	virtual void DrawRectangle(Vector2 p1, float width, float height) = 0;
	virtual void DrawRectangle(Vector2 p1, Vector2 p2) = 0;
	// draw a sprite
	virtual void DrawSprite(Vector2 p1, float width, float height, const char* texturePath) = 0;

	// Render a frame
	virtual void Render() = 0;
	// Shutdown the renderer
	virtual void Shutdown() = 0;
};