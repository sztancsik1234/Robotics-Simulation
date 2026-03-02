#include "pch.h"
#include "core/GameObject.h"
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

    std::string ToString() const override {
        return "MockComponent";
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
    GameObject gameObject(logger, id, Transform{}, name);

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
        
        // At this point, no components should be destroyed yet
        EXPECT_EQ(destructorCallCount, 0);
    } // GameObject goes out of scope here
    
    // After GameObject goes out of scope, all components should be destroyed
    EXPECT_EQ(destructorCallCount, 1);
}

TEST(GameObjectTest, EmplaceComponent)
{
    // Arrange
    MockLogger logger;
    GameObject gameObject(logger);
    
    // Act
    auto* component = gameObject.EmplaceComponent<MockComponent>();
    
    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_TRUE(component->wasAdded); // OnAdd should be called
    EXPECT_FALSE(component->wasUpdated); // Update hasn't been called yet
}

TEST(GameObjectTest, ComponentUpdate)
{
    // Arrange
    MockLogger logger;
    GameObject gameObject(logger);
    auto* component = gameObject.EmplaceComponent<MockComponent>();
    
    // Verify component is not updated initially
    EXPECT_FALSE(component->wasUpdated);
    
    // Act
    gameObject.Update();
    
    // Assert
    EXPECT_TRUE(component->wasUpdated);
}

TEST(GameObjectTest, GetComponent)
{
    // Arrange
    MockLogger logger;
    GameObject gameObject(logger);
    auto* originalComponent = gameObject.EmplaceComponent<MockComponent>();
    
    // Act
    auto* retrievedComponent = gameObject.GetComponent<MockComponent>();
    
    // Assert
    EXPECT_EQ(retrievedComponent, originalComponent);
}

TEST(GameObjectTest, GetNonexistentComponent)
{
    // Arrange
    MockLogger logger;
    GameObject gameObject(logger);
    // Don't add any MockComponent
    
    // Act & Assert
    EXPECT_THROW(gameObject.GetComponent<MockComponent>(), ComponentNotFoundException);
}

TEST(GameObjectTest, MoveGameObjectWithComponents)
{
    // Arrange
    MockLogger logger;
    GameObject sourceObj(logger, 1, Transform {}, "SourceObject");
    auto* component = sourceObj.EmplaceComponent<MockComponent>();
    
    // Act
    GameObject movedObj(std::move(sourceObj));
    
    // Assert
    auto* retrievedComponent = movedObj.GetComponent<MockComponent>();
    EXPECT_NE(retrievedComponent, nullptr);
    EXPECT_EQ(retrievedComponent, component);
    EXPECT_TRUE(retrievedComponent->wasAdded);
    EXPECT_EQ(movedObj.GetId(), 1);
    EXPECT_EQ(movedObj.GetName(), "SourceObject");
    
    // Source object should be reset to default state
    EXPECT_EQ(sourceObj.GetId(), 0);
    EXPECT_EQ(sourceObj.GetName(), "Unnamed");
    EXPECT_THROW(sourceObj.GetComponent<MockComponent>(), ComponentNotFoundException);
}

TEST(GameObjectTest, SetAndGetPosition)
{
    // Arrange
    MockLogger logger;
    GameObject gameObject(logger);
    Vector2 position(10.0f, 20.0f);
    
    // Act
    gameObject.SetPosition(position);
    
    // Assert
    EXPECT_EQ(gameObject.GetPosition().x, position.x);
    EXPECT_EQ(gameObject.GetPosition().y, position.y);
}

TEST(GameObjectTest, GetIdAndName)
{
    // Arrange
    MockLogger logger;
    int id = 42;
    std::string name = "GameObjectName";
    
    // Act
    GameObject gameObject(logger, id, Transform {}, name);
    
    // Assert
    EXPECT_EQ(gameObject.GetId(), id);
    EXPECT_EQ(gameObject.GetName(), name);
}

TEST(GameObjectTest, AddMultipleComponentsOfSameType)
{
    // Arrange
    MockLogger logger;
    GameObject gameObject(logger);
    
    // assert
    EXPECT_THROW(
        {
            auto* first = gameObject.EmplaceComponent<MockComponent>();
            auto* second = gameObject.EmplaceComponent<MockComponent>();
        },
        DuplicateComponentException);
}

TEST(GameObjectTest, AddComponentDirectly)
{
    // Arrange
    MockLogger logger;
    GameObject gameObject(logger);
    auto component = std::make_unique<MockComponent>(&gameObject);
    
    // Act
    gameObject.AddComponent(std::move(component));
    
    // Assert
    auto* retrieved = gameObject.GetComponent<MockComponent>();
    EXPECT_NE(retrieved, nullptr);
    EXPECT_TRUE(retrieved->wasAdded);
}


