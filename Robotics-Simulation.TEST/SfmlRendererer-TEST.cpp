// Unit tests for the SfmlRenderer class
#include "pch.h"
#include "graphics/SfmlRenderer.h"
#include "core/Vector2.h"

// Test fixture for SfmlRenderer
class SfmlRendererTest : public ::testing::Test {
protected:
    SfmlRenderer renderer;
};

TEST_F(SfmlRendererTest, CanInitializeAndShutdown) {
    EXPECT_NO_THROW(renderer.Initialize());
    EXPECT_NO_THROW(renderer.Shutdown());
}

TEST_F(SfmlRendererTest, CanClearAndRender) {
    renderer.Initialize();
    EXPECT_NO_THROW(renderer.Clear(Color::Black));
    EXPECT_NO_THROW(renderer.DisplayFrame());
    renderer.Shutdown();
}

TEST_F(SfmlRendererTest, DrawCircleDoesNotThrow) {
    renderer.Initialize();
    Vector2 center(100.0f, 100.0f);
    EXPECT_NO_THROW(renderer.DrawCircle(center, 20.0f));
    renderer.Shutdown();
}

TEST_F(SfmlRendererTest, DrawRectangleWithWidthHeightDoesNotThrow) {
    renderer.Initialize();
    Vector2 topLeft(50.0f, 50.0f);
    EXPECT_NO_THROW(renderer.DrawRectangle(topLeft, 40.0f, 30.0f));
    renderer.Shutdown();
}

TEST_F(SfmlRendererTest, DrawRectangleWithTwoPointsDoesNotThrow) {
    renderer.Initialize();
    Vector2 p1(10.0f, 10.0f);
    Vector2 p2(60.0f, 40.0f);
    EXPECT_NO_THROW(renderer.DrawRectangle(p1, p2));
    renderer.Shutdown();
}

TEST_F(SfmlRendererTest, DrawLineThrowsNotImplemented) {
    renderer.Initialize();
    Vector2 p(0.0f, 0.0f);
	Vector2 p2(100.0f, 100.0f);
    EXPECT_THROW(renderer.DrawLine(p, p2), NotImplementedException);
    renderer.Shutdown();
}

TEST_F(SfmlRendererTest, DrawSpriteThrowsNotImplemented) {
    renderer.Initialize();
    Vector2 p(0.0f, 0.0f);
    EXPECT_THROW(renderer.DrawSprite(p, 10.0f, 10.0f, "texture.png"), NotImplementedException);
    renderer.Shutdown();
}

