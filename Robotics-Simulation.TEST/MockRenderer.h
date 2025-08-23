#pragma once
#include "graphics/IRenderer.h"


// mock renderer for testing
class MockRenderer : public IPrimitiveRenderer, public ISpriteRenderer {
public:
    bool drawCircleCalled = false;
	bool loadTextureCalled = false;
	bool unloadTextureCalled = false;
    Vector2 lastPosition;
    float lastRadius = 0.0f;
	unsigned int lastTextureId = 0;
	Vector2 lastScale = { 1.f, 1.f };

    void DrawCircle(Vector2 position, float radius) override;
    void DrawLine(Vector2 start, Vector2 end) override;
    void DrawRectangle(Vector2 topLeft, float width, float height) override;
    void DrawRectangle(Vector2 p1, Vector2 p2) override;
    void DrawSprite(Vector2 position, int textureId, Vector2 scale = { 1.f, 1.f }) override;
	unsigned int LoadTexture(const char* filePath) override;
	unsigned int LoadTexture() override;
	void UnloadTexture(int textureId) override;
};
