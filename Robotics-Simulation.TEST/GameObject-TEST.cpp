#include "pch.h"
#include "GameObject.h"
#include <memory>
#include <string>

// Mock logger for testing
class MockLogger : public ILogger {
public:
    void Log(const std::string&, LogLevel) override {}
};

// Static counter to track component destructions
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
    
    ~MockComponent() {
        destructorCallCount++;
    }
};

TEST(GameObjectTest, Creation)
{
    // Arrange
    MockLogger logger;
    std::string name = "TestObject";
    int id = 1;
    // Act
    //GameObject gameObject(logger, id, name);
    //// Assert

    GameObject gameObject(logger, id, name);

    //EXPECT_EQ(gameObject.GetId(), id);
    //EXPECT_EQ(gameObject.GetName(), name);
}

TEST(GameObjectTest, ResourceCleanup) {
    //// Reset the counter before test
    //destructorCallCount = 0;
    //
    //{
    //    // Create a scope for GameObject
    //    MockLogger logger;
    //    GameObject gameObject(logger);
    //    
    //    // Add components that should be cleaned up when gameObject goes out of scope
    //    gameObject.EmplaceComponent<MockComponent>();
    //    gameObject.EmplaceComponent<MockComponent>();
    //    gameObject.EmplaceComponent<MockComponent>();
    //    
    //    // At this point, no components should be destroyed yet
    //    EXPECT_EQ(destructorCallCount, 0);
    //} // GameObject goes out of scope here
    //
    //// After GameObject goes out of scope, all components should be destroyed
    //EXPECT_EQ(destructorCallCount, 3);
}
