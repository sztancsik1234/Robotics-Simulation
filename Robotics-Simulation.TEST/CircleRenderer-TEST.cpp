#include "pch.h"
#include "CircleRenderer.h"
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
    
    Vector2 testPosition(10.0f, 20.0f);
    gameObject->SetPosition(testPosition);
    
    CircleRenderer circleRenderer(gameObject.get(), renderer, logger);
    
    // Act
    circleRenderer.Update();
    
    // Assert
    EXPECT_TRUE(renderer.drawCircleCalled);
    EXPECT_FLOAT_EQ(renderer.lastPosition.x, testPosition.x);
    EXPECT_FLOAT_EQ(renderer.lastPosition.y, testPosition.y);
    EXPECT_FLOAT_EQ(renderer.lastRadius, 50.0f);
}