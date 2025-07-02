#pragma once
#include "IRenderer.h"

class GlfwRenderer : public IRenderer
{
	// Inherited via IRenderer
	void Initialize() override;
	void DrawLine(Vector2 p) override;
	void DrawCircle(Vector2 p, float radius) override;
	void DrawRectangle(Vector2 p1, float width, float height) override;
	void DrawRectangle(Vector2 p1, Vector2 p2) override;
	void DrawSprite(Vector2 p1, float width, float height, const char* texturePath) override;
	void Render() override;
	void Shutdown() override;
};

