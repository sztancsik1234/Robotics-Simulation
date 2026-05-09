#include "pch.h"
#include "graphics/CircleRendererComponent.h"
#include "gtest/gtest.h"
#include <memory>
#include "MockLogger.h"
#include "MockCameraRenderer.h"
#include "MockGameObject.h"

TEST(CircleRendererTest, UpdateDrawsCircleAtCorrectPosition) {
    // Arrange
    MockLogger logger;
    MockCameraRenderer camera;
    auto gameObject = std::make_unique<MockGameObject>(logger);

    Vector2 testPosition(10, 15);
    gameObject->SetPosition(testPosition);

    CircleRendererComponent circleRenderer(gameObject.get(), camera, logger);
    circleRenderer.OnAdd();

    // Act
    circleRenderer.Update();

    // Assert
    EXPECT_TRUE(camera.drawCircleCalled);
    EXPECT_FLOAT_EQ(camera.lastWorldCenter.x, testPosition.x);
    EXPECT_FLOAT_EQ(camera.lastWorldCenter.y, testPosition.y);
}

TEST(CircleRendererTest, UpdateDrawsCircleWithCorrectRadius) {
    // Arrange
    MockLogger logger;
    MockCameraRenderer camera;
    auto gameObject = std::make_unique<MockGameObject>(logger);

    CircleRendererComponent circleRenderer(gameObject.get(), camera, logger);
    circleRenderer.OnAdd();

    // Act
    circleRenderer.Update();

    // Assert
    EXPECT_TRUE(camera.drawCircleCalled);
    EXPECT_FLOAT_EQ(camera.lastWorldRadius, 10.0f);
}