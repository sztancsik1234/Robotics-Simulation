#include "pch.h"
#include "graphics/Camera.h"
#include "input/MouseFollowerComponent.h"
#include "gtest/gtest.h"
#include <memory>
#include "MockLogger.h"
#include "MockInputService.h"
#include "MockGameObject.h"
#include "MockViewport.h"

class MouseFollowerComponentTest : public ::testing::Test {
protected:
    MockLogger logger;
    MockInputService mockInput;
	MockViewport mockViewport;
    std::unique_ptr<MockGameObject> gameObject;
    std::unique_ptr<MouseFollowerComponent> component;

    void SetUp() override {
        gameObject = std::make_unique<MockGameObject>(logger);
        component = std::make_unique<MouseFollowerComponent>(gameObject.get(), mockViewport, mockInput);
    }
};

TEST_F(MouseFollowerComponentTest, UpdateCallsGetMousePosition) {
    // Arrange
    mockInput.mousePosition = Vector2(50.0f, 60.0f);
    
    // Act
    component->Update();
    
    // Assert
    EXPECT_TRUE(mockInput.getMousePositionCalled);
}

TEST_F(MouseFollowerComponentTest, UpdateSetsGameObjectPositionToMousePosition) {
    // Arrange
    Vector2 expectedPosition(100.0f, 200.0f);
    mockInput.mousePosition = expectedPosition;
    
    // Act
    component->Update();
    
    // Assert
    Vector2 actualPosition = gameObject->GetPosition();
    EXPECT_FLOAT_EQ(actualPosition.x, expectedPosition.x);
    EXPECT_FLOAT_EQ(actualPosition.y, expectedPosition.y);
}

TEST_F(MouseFollowerComponentTest, HandlesDifferentMousePositions) {
    // Test case 1: Origin
    Vector2 position1(0.0f, 0.0f);
    mockInput.mousePosition = position1;
    component->Update();
    Vector2 actual1 = gameObject->GetPosition();
    EXPECT_FLOAT_EQ(actual1.x, position1.x);
    EXPECT_FLOAT_EQ(actual1.y, position1.y);
    
    // Test case 2: Negative coordinates
    Vector2 position2(-50.0f, -75.0f);
    mockInput.mousePosition = position2;
    component->Update();
    Vector2 actual2 = gameObject->GetPosition();
    EXPECT_FLOAT_EQ(actual2.x, position2.x);
    EXPECT_FLOAT_EQ(actual2.y, position2.y);
    
    // Test case 3: Large coordinates
    Vector2 position3(9999.9f, 8888.8f);
    mockInput.mousePosition = position3;
    component->Update();
    Vector2 actual3 = gameObject->GetPosition();
    EXPECT_FLOAT_EQ(actual3.x, position3.x);
    EXPECT_FLOAT_EQ(actual3.y, position3.y);
}

TEST_F(MouseFollowerComponentTest, OnAddAndOnRemoveDoNotCrash) {
    // These methods are empty in the implementation but we should test
    // that they don't crash when called
    EXPECT_NO_THROW(component->OnAdd());
    EXPECT_NO_THROW(component->OnRemove());
}