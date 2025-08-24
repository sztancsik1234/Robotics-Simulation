#pragma once
#include "graphics/IRenderer.h"


// mock renderer for testing
class MockRenderer : public IPrimitiveRenderer, public ISpriteRenderer {
public:
    bool drawCircleCalled = false;
	bool loadTextureCalled = false;
	bool unloadTextureCalled = false;
	bool drawSpriteCalled = false;
    Vector2 lastPosition;
    float lastRadius = 0.0f;
	unsigned int lastTextureId = 0;
	Vector2 lastSize = { 1.f, 1.f };
	Vector2 lastAncor = { 0.f, 0.f };
	static constexpr unsigned int DummyTextureId = 66;
	static constexpr unsigned int DummyDefaultTextureId = 33;

    void DrawCircle(Vector2 center, float radius) override;
    void DrawLine(Vector2 start, Vector2 end) override;
    void DrawRectangle(Vector2 topLeft, float width, float height) override;
    void DrawRectangle(Vector2 p1, Vector2 p2) override;
    void DrawSprite(Vector2 position, unsigned int textureId, Vector2 size, const Vector2 SpriteAncor = { 0.f, 0.f }) override;
	unsigned int LoadTexture(const char* filePath) override;
	unsigned int LoadTexture() override;
	void UnloadTexture(unsigned int textureId) override;
};
