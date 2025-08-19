#include "pch.h"
#include "CircleRenderer.h"
#include "gtest/gtest.h"
#include <memory>

// Mock renderer for testing
class MockRenderer : public IDrawableRenderer {
public:
    bool drawCircleCalled = false;
    Vector2 lastPosition;
    float lastRadius = 0.0f;

    void DrawCircle(Vector2 position, float radius) override {
        drawCircleCalled = true;
        lastPosition = position;
        lastRadius = radius;
    }

    void DrawLine(Vector2 start, Vector2 end) override {
        throw NotImplementedException();
	}

    void DrawRectangle(Vector2 topLeft, float width, float height) override {
        throw NotImplementedException();
    }
    void DrawRectangle(Vector2 p1, Vector2 p2) override {
        throw NotImplementedException();
    }
    void DrawSprite(Vector2 position, float width, float height, const char* texturePath) override {
        throw NotImplementedException();
	}
};

// Mock logger for testing
class MockLogger : public ILogger {
public:
    std::string lastMessage;
    LogLevel lastLevel;
    bool logCalled = false;

    void Log(const std::string& message, LogLevel level) override {
        lastMessage = message;
        lastLevel = level;
        logCalled = true;
    }
};

// Mock GameObject for testing
class MockGameObject : public GameObject {
public:
    MockGameObject(ILogger& logger) : GameObject(logger) {}
};

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