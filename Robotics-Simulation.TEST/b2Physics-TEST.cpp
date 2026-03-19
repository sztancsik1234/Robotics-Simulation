#include "pch.h"
#include <gtest/gtest.h> 
#include "physics/b2Physics.h"
#include "core/Vector2.h"
#include "MockLogger.h"
#include <random>

class B2PhysicsTest : public ::testing::Test
{ 
protected: 
    MockLogger logger; 
    b2Physics physics = b2Physics(logger);

    void SetUp() override {
        // Any common setup can be done here, but for now we will initialize and shutdown in each test to ensure isolation.
		physics.Initialize();
    }

    void TearDown() override
    {
        physics.Shutdown();
    }
};



TEST_F(B2PhysicsTest, SimulateStepDoesNotThrow)
{
    EXPECT_NO_THROW(physics.simulateStep(1.0f / 60.0f)); 
}

TEST_F(B2PhysicsTest, CreateCircleBodyDoesNotThrow) {
    BodyDefinition def;
    def.type = BodyType::DYNAMIC;
    def.position = Vector2(0.0f, 0.0f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::CIRCLE;
    def.shape.circle.radius = 1.0f;

    EXPECT_NO_THROW({
        auto id = physics.CreateBody(def);
        (void)id;
        });

}

TEST_F(B2PhysicsTest, CreateRectangleBodyDoesNotThrow) {
    BodyDefinition def;
    def.type = BodyType::DYNAMIC;
    def.position = Vector2(0.0f, 0.0f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::RECTANGLE;
    def.shape.rectangle.width = 2.0f;
    def.shape.rectangle.height = 1.0f;

    EXPECT_NO_THROW({
        auto id = physics.CreateBody(def);
        (void)id;
        });

}

TEST_F(B2PhysicsTest, CreateRectangleBodyUsesInitialPosition) {
    // Arrange
    BodyDefinition def;
    def.type = BodyType::DYNAMIC;
    def.position = Vector2(3.5f, -2.25f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::RECTANGLE;
    def.shape.rectangle.width = 4.0f;
    def.shape.rectangle.height = 1.5f;

    // Act
    auto id = physics.CreateBody(def);
    Vector2 pos = physics.GetBodyPosition(id);

    // Assert
    EXPECT_NEAR(pos.x, def.position.x, 0.001f);
    EXPECT_NEAR(pos.y, def.position.y, 0.001f);
}

TEST_F(B2PhysicsTest, CreateBodyWithInvalidShapeThrows) {
    BodyDefinition def;
    def.type = BodyType::DYNAMIC;
    def.position = Vector2(0.0f, 0.0f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::NONE; // invalid for creation

    EXPECT_THROW(physics.CreateBody(def), PhysicsObjectCreationFailedException);

}

TEST_F(B2PhysicsTest, SimulationTest)
{
	// create a circle, check if it falls under gravity
	// First, create a dynamic body with a circle shape

	Vector2 initialPosition(0.0f, 10.0f); // Start 10 units above the ground

    BodyDefinition def;
    def.type = BodyType::DYNAMIC;
    def.position = initialPosition; // Start 10 units above the ground
    def.rotation = Radian(0.0f);
	def.SetShapeCircle(1.0f); // Set shape to circle with radius 1.0f
    def.shape.circle.radius = 1.0f;
	def.density = 1.0f; // Give it some mass so gravity affects it
    auto bodyId = physics.CreateBody(def);
    // Simulate for 1 second in 60 steps
    for (int i = 0; i < 60; ++i) {
        physics.simulateStep(1.0f / 60.0f);
    }
    // Get the final position of the body
    Vector2 finalPosition = physics.GetBodyPosition(bodyId);
    // The body should have fallen down, so its y position should be less than the initial position
    EXPECT_LT(finalPosition.y, def.position.y);
}

TEST_F(B2PhysicsTest, AccuracyTest)
{
	// Create a circle body, make it fall for a while and check if the final position is close to the expected position under gravity
    // For reference: under 1 second, a body falls 4.9m-s. Disrgarding air resistance, like we do.
	Vector2 initialPosition(0.0f, 0.0f); // Start at 0


    BodyDefinition def {
        .position = initialPosition,
        .type = BodyType::DYNAMIC,
        .density = 1.0f
    };
	def.SetShapeCircle(1.0f); // Set shape to circle with radius 1.0f
    
    auto bodyId = physics.CreateBody(def);

    // Simulate for 1 second in 60 steps
    for (int i = 0; i < 60; ++i) {
        physics.simulateStep(1.0f / 60.0f);
    }

    // Get the final position of the body
    Vector2 finalPosition = physics.GetBodyPosition(bodyId);

    // The body should have fallen down approximately 4.9 units under gravity
	EXPECT_NEAR(finalPosition.y, -4.9f, 0.1f); // Allow a small margin of error
}


TEST_F(B2PhysicsTest, MultipleObjectsSimulatedTest)
{
	// just like previously, but with multiple objects. We want to make sure that the physics engine can handle multiple bodies and that they all fall under gravity as expected.
	// create 10 bodies at random locations
	auto randomEngine = std::default_random_engine {};
	Vector2 initialPositions[10];
	Vector2 expectedPositions[10];
	BodyId bodyIds[10];

    for (int i = 0; i < 10; i++)
    {
		float x = std::uniform_real_distribution<float> { -70.0f, 70.0f }(randomEngine);
		float y = std::uniform_real_distribution<float> { -70.0f, 70.0f }(randomEngine);
        initialPositions[i] = { x, y };
		expectedPositions[i] = { x, y - 4.9f }; // they should all fall approximately 4.9 units under gravity

		// create body definition
		BodyDefinition def {
			.position = initialPositions[i],
            .type = BodyType::DYNAMIC,
			.density = 1.0f
		};
		def.SetShapeCircle(1.0f); // Set shape to circle with radius 1.0f
		bodyIds[i] = physics.CreateBody(def);
    }

	// Simulate for 1 second in 60 steps
    for (int i = 0; i < 60; ++i) {
        physics.simulateStep(1.0f / 60.0f);
	}

    // Get the final position of each body and check if it's close to the expected position
    for (int i = 0; i < 10; i++)
    {
        Vector2 finalPosition = physics.GetBodyPosition(bodyIds[i]);
        EXPECT_NEAR(finalPosition.x, expectedPositions[i].x, 0.1f);
        EXPECT_NEAR(finalPosition.y, expectedPositions[i].y, 0.1f); 
	}
}

TEST_F(B2PhysicsTest, SetShapeCircleTest)
{
    BodyDefinition circleDef;
	int radius = 1.3f;

    circleDef.SetShapeCircle(radius);

    EXPECT_EQ(circleDef.shapeType, ShapeType::CIRCLE);
    EXPECT_EQ(circleDef.shape.circle.radius, radius);
}

TEST_F(B2PhysicsTest, SetShapeRectangleTest)
{
    BodyDefinition rectDef;
    Vector2 size = { 2.0f, 3.0f };

    rectDef.SetShapeRectangle(size);

    EXPECT_EQ(rectDef.shapeType, ShapeType::RECTANGLE);
    EXPECT_EQ(rectDef.shape.rectangle.width, size.x);
    EXPECT_EQ(rectDef.shape.rectangle.height, size.y);
}

TEST_F(B2PhysicsTest, BallPlatformCollisionMinYTest)
{
    const     Vector2 platformPosition  = { 0.0f, -2.25f };
    const     Vector2 ballPosition      = { 0.0f,  2.0f  };
    constexpr float   platformWidth     = 10.0f;
    constexpr float   platformHeight    = 0.5f;
    constexpr float   ballRadius        = 0.5f;
    constexpr float   minYThreshold     = -2.23f;

#if 0   // This one works:
    // Arrange: static platform
    const BodyDefinition platformDef {
        .position  = platformPosition,
        .rotation  = Radian(0.0f),
        .type      = BodyType::STATIC,
        .density   = 0.0f,
        .shapeType = ShapeType::RECTANGLE,
        .shape     = { .rectangle = { platformWidth, platformHeight } }
    };

    physics.CreateBody(platformDef);

    // Arrange: dynamic ball
    const BodyDefinition ballDef {
        .position  = ballPosition,
        .rotation  = Radian(0.0f),
        .type      = BodyType::DYNAMIC,
        .density   = 1.0f,
        .shapeType = ShapeType::CIRCLE,
        .shape     = { .circle = { ballRadius } }
    };

#else   // This one doesn't:
    // 03-19-20:28 - NVM, it works again
    // Arrange: static platform
    BodyDefinition platformDef {};
    platformDef.type     = BodyType::STATIC;
    platformDef.position = platformPosition;
    platformDef.rotation = Radian(0.0f);
    platformDef.SetShapeRectangle(Vector2(platformWidth, platformHeight));
    platformDef.density  = 0.0f;

    physics.CreateBody(platformDef);

    // Arrange: dynamic ball
    BodyDefinition ballDef;
    ballDef.type     = BodyType::DYNAMIC;
    ballDef.position = ballPosition;
    ballDef.rotation = Radian(0.0f);
    ballDef.SetShapeCircle(ballRadius);
    ballDef.density  = 1.0f;
#endif

    const auto ballId = physics.CreateBody(ballDef);

    // Act: simulate 3 seconds and track minimum y
    float minY = ballDef.position.y;
    for (int i = 0; i < 180; ++i)
    {
        physics.simulateStep(1.0f / 60.0f);
        Vector2 pos = physics.GetBodyPosition(ballId);
        if (pos.y < minY) minY = pos.y;
    }

    // Assert: ball doesn't go below minYThreshold
    EXPECT_GE(minY, minYThreshold);
}

TEST_F(B2PhysicsTest, StaticBoxCreatedAtCorrectPosition)
{
    // Arrange
    BodyDefinition def;
    def.type = BodyType::STATIC;
    def.position = Vector2(25.0f, -25.0f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::RECTANGLE;
    def.shape.rectangle.width = 2.0f;
    def.shape.rectangle.height = 1.0f;

    // Act
    auto id = physics.CreateBody(def);
    Vector2 pos = physics.GetBodyPosition(id);

    // Assert
    EXPECT_NEAR(pos.x, def.position.x, 0.001f);
    EXPECT_NEAR(pos.y, def.position.y, 0.001f);
}

TEST_F(B2PhysicsTest, StaticBoxStaysAtPositionAfterStep)
{
    // Arrange
    BodyDefinition def;
    def.type = BodyType::STATIC;
    def.position = Vector2(32.0f, 100.0f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::RECTANGLE;
    def.shape.rectangle.width = 2.0f;
    def.shape.rectangle.height = 1.0f;

    auto id = physics.CreateBody(def);

    // Act
    physics.simulateStep(1.0f / 60.0f);
    Vector2 pos = physics.GetBodyPosition(id);

    // Assert
    EXPECT_NEAR(pos.x, def.position.x, 0.001f);
    EXPECT_NEAR(pos.y, def.position.y, 0.001f);
}