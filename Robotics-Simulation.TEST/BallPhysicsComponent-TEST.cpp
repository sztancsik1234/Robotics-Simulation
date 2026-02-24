#include "pch.h"
#include <gtest/gtest.h>
#include <random>
#include <vector>

#include "physics/BallPhysicsComponent.h"
#include "physics/b2Physics.h"
#include "core/GameObject.h"
#include "MockLogger.h"
#include "MockPhysicsEngine.h"

class BallPhysicsComponentMockTest : public ::testing::Test
{
protected:
    MockLogger logger;
    MockPhysicsEngine physics;

    void SetUp() override
    {
        physics.Initialize();
    }

    void TearDown() override
    {
        physics.Shutdown();
    }
};

class BallPhysicsComponentB2Test : public ::testing::Test
{
protected:
    MockLogger logger;
    b2Physics physics = b2Physics(logger);

    void SetUp() override
    {
        physics.Initialize();
    }

    void TearDown() override
    {
        physics.Shutdown();
    }
};

TEST_F(BallPhysicsComponentMockTest, CreateBodyAndDestroyBodyAreCalled)
{
    // Arrange
    {
        GameObject gameObject(logger, 1, Transform{ .position = { 1.0f, 2.0f }, .size = { 2.0f, 2.0f } }, "Ball");

        // Act
        gameObject.EmplaceComponent<BallPhysicsComponent>(logger, physics);

        // Assert
        EXPECT_TRUE(physics.createBodyCalled);
    }

    // Assert (after GameObject destruction triggers OnRemove)
    EXPECT_TRUE(physics.destroyBodyCalled);
}

TEST_F(BallPhysicsComponentMockTest, CreateBodyUsesGameObjectCoordinates)
{
    // Arrange
    Transform transform{};
    transform.position = { 3.5f, -1.25f };
    transform.size = { 2.0f, 2.0f };
    GameObject gameObject(logger, 1, transform, "Ball");

    // Act
    gameObject.EmplaceComponent<BallPhysicsComponent>(logger, physics);

    // Assert
    Vector2 createdPosition = physics.getBodyPosition(physics.lastBodyId);
    EXPECT_FLOAT_EQ(createdPosition.x, transform.position.x);
    EXPECT_FLOAT_EQ(createdPosition.y, transform.position.y);
}

TEST_F(BallPhysicsComponentB2Test, BallFallsUnderGravityAfterOneSecond)
{
    // Arrange
    GameObject gameObject(logger, 1, Transform{ .position = { 0.0f, 0.0f }, .size = { 2.0f, 2.0f } }, "Ball");
    gameObject.EmplaceComponent<BallPhysicsComponent>(logger, physics);

    // Act
    for (int i = 0; i < 60; ++i)
    {
        physics.simulateStep(1.0f / 60.0f);
    }
    gameObject.Update();

    // Assert
    Vector2 finalPos = gameObject.GetPosition();
    EXPECT_NEAR(finalPos.x, 0.0f, 0.1f);
    EXPECT_NEAR(finalPos.y, -4.9f, 0.2f);
}

TEST_F(BallPhysicsComponentB2Test, MultipleBallsFallApproximatelyExpectedDistance)
{
    // Arrange
    std::default_random_engine randomEngine{ 42 };
    std::uniform_real_distribution<float> dist(-70.0f, 70.0f);

    constexpr int BallCount = 10;
    std::vector<Vector2> initialPositions;
    std::vector<std::unique_ptr<GameObject>> gameObjects;
    initialPositions.reserve(BallCount);
    gameObjects.reserve(BallCount);

    for (int i = 0; i < BallCount; ++i)
    {
        Vector2 pos{ dist(randomEngine), dist(randomEngine) };
        initialPositions.push_back(pos);

        Transform transform{};
        transform.position = pos;
        transform.size = { 2.0f, 2.0f };

        auto gameObject = std::make_unique<GameObject>(logger, i + 1, transform, "Ball");
        gameObject->EmplaceComponent<BallPhysicsComponent>(logger, physics);
        gameObjects.push_back(std::move(gameObject));
    }

    // Act
    for (int i = 0; i < 60; ++i)
    {
        physics.simulateStep(1.0f / 60.0f);
    }
    for (auto const& obj : gameObjects)
    {
        obj->Update();
    }

    // Assert
    int successCount = 0;
    for (int i = 0; i < BallCount; ++i)
    {
        Vector2 finalPos = gameObjects[i]->GetPosition();
        float expectedY = initialPositions[i].y - 4.9f;
        if (std::abs(finalPos.y - expectedY) <= 0.2f)
        {
            successCount++;
        }
    }
    EXPECT_GE(successCount, 8);
}