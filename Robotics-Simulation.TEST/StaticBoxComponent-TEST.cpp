#include "pch.h"
#include "physics/StaticBoxComponent.h"
#include "MockLogger.h"
#include "MockGameObject.h"
#include "MockPhysicsEngine.h"
#include "gtest/gtest.h"

TEST(StaticBoxComponentTests, OnAdd_CreatesBody)
{
    // Arrange
    MockLogger logger;
    MockPhysicsEngine physics;
    physics.Initialize();

    MockGameObject owner(logger);
    StaticBoxComponent sut(&owner, logger, dynamic_cast<IBodyManager&>(physics));

    // Act
    sut.OnAdd();

    // Assert
    EXPECT_TRUE(physics.createBodyCalled);
    EXPECT_GT(physics.lastBodyId, static_cast<BodyId>(0));

    physics.Shutdown();
}

TEST(StaticBoxComponentTests, Update_DoesNothing)
{
    // Arrange
    MockLogger logger;
    MockPhysicsEngine physics;
    physics.Initialize();

    MockGameObject owner(logger);
    StaticBoxComponent sut(&owner, logger, dynamic_cast<IBodyManager&>(physics));
    sut.OnAdd();

    // Sanity: ensure flags start false
    EXPECT_FALSE(physics.applyForceCalled);
    EXPECT_FALSE(physics.setSpeedCalled);

    // Act
    sut.Update();

    // Assert
    EXPECT_FALSE(physics.applyForceCalled);
    EXPECT_FALSE(physics.setSpeedCalled);

    physics.Shutdown();
}

TEST(StaticBoxComponentTests, OnRemove_DestroysBody)
{
    // Arrange
    MockLogger logger;
    MockPhysicsEngine physics;
    physics.Initialize();

    MockGameObject owner(logger);
    StaticBoxComponent sut(&owner, logger, dynamic_cast<IBodyManager&>(physics));
    sut.OnAdd();

    // Act
    sut.OnRemove();

    // Assert
    EXPECT_TRUE(physics.destroyBodyCalled);

    physics.Shutdown();
}

TEST(StaticBoxComponentTests, ToString_ReturnsExpected)
{
    // Arrange
    MockLogger logger;
    MockPhysicsEngine physics;
    physics.Initialize();

    MockGameObject owner(logger);
    StaticBoxComponent sut(&owner, logger, dynamic_cast<IBodyManager&>(physics));
	std::string classname = "StaticBoxComponent";
	std::string expected = std::format("<{}>", classname);

    // Act
    const std::string text = sut.ToString();

    // Assert
    EXPECT_EQ(text, expected);

    physics.Shutdown();
}

TEST(StaticBoxComponentTests, PhysicsBodyTransformIsOwnerTransform)
{
    // Arrange
    MockLogger logger;
    MockPhysicsEngine physics;
    physics.Initialize();
    MockGameObject owner(logger);
    Vector2 expectedPosition(10.0f, 20.0f);
	Radian expectedRotation(0.5f);
    owner.SetPosition(expectedPosition);
	owner.SetRotation(expectedRotation);
    StaticBoxComponent sut(&owner, logger, dynamic_cast<IBodyManager&>(physics));

    // Act
    sut.OnAdd();

    // Assert
    Vector2 bodyPosition = physics.getBodyPosition(physics.lastBodyId);
    EXPECT_FLOAT_EQ(bodyPosition.x, expectedPosition.x);
    EXPECT_FLOAT_EQ(bodyPosition.y, expectedPosition.y);
    EXPECT_DOUBLE_EQ(physics.getBodyRotation(physics.lastBodyId).toRadian(), expectedRotation.toRadian());
	physics.Shutdown();
}
