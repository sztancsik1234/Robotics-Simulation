#include "pch.h"
#include "GameObject.h"
#include <memory>
#include <string>
#include "MockLogger.h"

// Static counter to track component destructions
// This is intentionally non-const to allow modification during tests
static int destructorCallCount = 0;

// Mock component for testing
class MockComponent : public Component {
public:
    using Component::Component;
    bool wasUpdated = false;
    bool wasAdded = false;

    void Update() override {
        wasUpdated = true;
    }

    void OnAdd() override {
        wasAdded = true;
    }

    void OnRemove() override {
        // No specific action needed for this mock
    }
    
    ~MockComponent() override {
        destructorCallCount++;
    }

    // Copy operations are deleted since Component should not be copyable
    // due to ownership semantics with GameObject
    MockComponent(const MockComponent&) = delete;
    MockComponent& operator=(const MockComponent&) = delete;
    
    // Move operations are deleted for the same reason
    MockComponent(MockComponent&&) = delete;
    MockComponent& operator=(MockComponent&&) = delete;
};

TEST(GameObjectTest, Creation)
{
    // Arrange
    MockLogger logger;
    std::string name = "TestObject";
    int id = 1;

    // Act
    GameObject gameObject(logger, id, name);

    // Assert
    EXPECT_EQ(gameObject.GetId(), id);
    EXPECT_EQ(gameObject.GetName(), name);
}

TEST(GameObjectTest, ResourceCleanup) {
    // Reset the counter before test
    destructorCallCount = 0;
    
    {
        // Create a scope for GameObject
        MockLogger logger;
        auto gameObject = std::make_unique<GameObject>(logger);
        
        // Add components that should be cleaned up when gameObject goes out of scope
        gameObject->EmplaceComponent<MockComponent>();
        gameObject->EmplaceComponent<MockComponent>();
        gameObject->EmplaceComponent<MockComponent>();
        
        // At this point, no components should be destroyed yet
        EXPECT_EQ(destructorCallCount, 0);
    } // GameObject goes out of scope here
    
    // After GameObject goes out of scope, all components should be destroyed
    EXPECT_EQ(destructorCallCount, 3);
}
