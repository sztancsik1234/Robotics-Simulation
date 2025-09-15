// Unit tests for the SfmlRenderer class
#include "pch.h"
#include "graphics/SfmlRenderer.h"
#include "core/Vector2.h"
#include "MockLogger.h"

// Test fixture for SfmlRenderer
class SfmlRendererTest : public ::testing::Test {
protected:
    MockLogger logger;
    SfmlRenderer renderer = SfmlRenderer(logger);
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

TEST_F(SfmlRendererTest, LoadTextureAssignsUniqueIds) {
    renderer.Initialize();
    unsigned int textureId1 = renderer.LoadTexture();
    unsigned int textureId2 = renderer.LoadTexture();
    EXPECT_NE(textureId1, textureId2);
    renderer.Shutdown();
}

