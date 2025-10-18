#include "pch.h"
#include <gtest/gtest.h> 
#include "physics/b2Physics.h"
#include "core/Vector2.h"
#include "MockLogger.h"

class B2PhysicsTest : public ::testing::Test
{ 
protected: 
    MockLogger logger; 
    b2Physics physics = b2Physics(logger); 
};

TEST_F(B2PhysicsTest, CanInitializeAndShutdown) 
{ 
    EXPECT_NO_THROW(physics.Initialize()); 
    EXPECT_NO_THROW(physics.Shutdown()); 
}

TEST_F(B2PhysicsTest, SimulateStepDoesNotThrow)
{
    physics.Initialize(); 
    EXPECT_NO_THROW(physics.simulateStep(1.0f / 60.0f)); 
    physics.Shutdown(); 
}

TEST_F(B2PhysicsTest, CreateCircleBodyDoesNotThrow) {
    physics.Initialize();
    BodyDefinition def;
    def.type = BodyType::Dynamic;
    def.position = Vector2(0.0f, 0.0f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::Circle;
    def.shape.circle.radius = 1.0f;

    EXPECT_NO_THROW({
        auto id = physics.createBody(def);
        (void)id;
        });

    physics.Shutdown();
}

TEST_F(B2PhysicsTest, CreateRectangleBodyDoesNotThrow) {
    physics.Initialize();
    BodyDefinition def;
    def.type = BodyType::Dynamic;
    def.position = Vector2(0.0f, 0.0f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::Rectangle;
    def.shape.rectangle.width = 2.0f;
    def.shape.rectangle.height = 1.0f;

    EXPECT_NO_THROW({
        auto id = physics.createBody(def);
        (void)id;
        });

    physics.Shutdown();
}

TEST_F(B2PhysicsTest, CreateBodyWithInvalidShapeThrows) {
    physics.Initialize();
    BodyDefinition def;
    def.type = BodyType::Dynamic;
    def.position = Vector2(0.0f, 0.0f);
    def.rotation = Radian(0.0f);
    def.shapeType = ShapeType::None; // invalid for creation

    EXPECT_THROW(physics.createBody(def), PhysicsObjectCreationFailedException);

    physics.Shutdown();
}
