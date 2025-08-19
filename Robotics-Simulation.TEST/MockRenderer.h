#pragma once
#include "IRenderer.h"


// mock renderer for testing
class MockRenderer : public IDrawableRenderer {
public:
    bool drawCircleCalled = false;
    Vector2 lastPosition;
    float lastRadius = 0.0f;

    void DrawCircle(Vector2 position, float radius) override;
    void DrawLine(Vector2 start, Vector2 end) override;
    void DrawRectangle(Vector2 topLeft, float width, float height) override;
    void DrawRectangle(Vector2 p1, Vector2 p2) override;
    void DrawSprite(Vector2 position, float width, float height, const char* texturePath) override;
};
