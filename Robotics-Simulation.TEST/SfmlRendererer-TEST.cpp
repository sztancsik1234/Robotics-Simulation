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

