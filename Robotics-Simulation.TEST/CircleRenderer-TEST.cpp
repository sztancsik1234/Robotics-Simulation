#include "pch.h"
#include "graphics/CircleRendererComponent.h"
#include "gtest/gtest.h"
#include <memory>
#include "MockLogger.h"
#include "MockRenderer.h"
#include "MockGameObject.h"



TEST(CircleRendererTest, UpdateDrawsCircleAtCorrectPosition) {
    // Arrange
    MockLogger logger;
    MockRenderer renderer;
    auto gameObject = std::make_unique<MockGameObject>(logger);
	Camera camera(renderer, logger, { 0, 0 }, { 800, 600 }, { 800, 600 });
    
    Vector2 testPosition(0, 0);

    // manually calculate testposition in pixel-space
	Vector2 EcpectedPixelPosition = camera.PixelToWorldPos(testPosition);

    gameObject->SetPosition(testPosition);
    
    CircleRendererComponent circleRenderer(gameObject.get(), camera, logger);
    
    // Act
    circleRenderer.Update();
    
    // Assert
    EXPECT_TRUE(renderer.drawCircleCalled);
    EXPECT_FLOAT_EQ(renderer.lastPosition.x, EcpectedPixelPosition.x);
    EXPECT_FLOAT_EQ(renderer.lastPosition.y, EcpectedPixelPosition.y);
    EXPECT_FLOAT_EQ(renderer.lastRadius, 50.0f);
}