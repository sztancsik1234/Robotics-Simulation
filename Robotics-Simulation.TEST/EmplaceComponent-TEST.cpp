#include "pch.h"
#include "core/GameObject.h"
#include <memory>
#include <string>
#include "MockLogger.h"

// Mock component with no additional constructor parameters
class SimpleComponent : public Component {
public:
    using Component::Component;
    bool wasAdded = false;
    bool wasUpdated = false;

    void OnAdd() override { wasAdded = true; }
    void Update() override { wasUpdated = true; }
    void OnRemove() override {}
    void Disable() override {}
    void Enable() override {}
	std::string ToString() const override { return "SimpleComponent"; }
    std::unique_ptr<Component> Clone(GameObject* newOwner) const override
    {
        return std::make_unique<SimpleComponent>(newOwner);
    }
};

// Mock component with one parameter constructor
class SingleParamComponent : public Component {
public:
    int value;
    bool wasAdded = false;

    SingleParamComponent(GameObject* owner, int val)
        : Component(owner), value(val) {}

    void OnAdd() override { wasAdded = true; }
    void Update() override {}
    void OnRemove() override {}
    void Disable() override {}
    void Enable() override {}
    std::string ToString() const override { return "SingleParamComponent"; }
    std::unique_ptr<Component> Clone(GameObject* newOwner) const override
    {
        return std::make_unique<SingleParamComponent>(newOwner, value);
    }
};

// Mock component with multiple parameter constructor
class MultiParamComponent : public Component {
public:
    int intValue;
    std::string stringValue;
    float floatValue;
    bool wasAdded = false;

    MultiParamComponent(GameObject* owner, int i, const std::string& s, float f)
        : Component(owner), intValue(i), stringValue(s), floatValue(f) {}

    void OnAdd() override { wasAdded = true; }
    void Update() override {}
    void OnRemove() override {}
    void Disable() override {}
    void Enable() override {}
    std::string ToString() const override { return "MultiParamComponent"; }
    std::unique_ptr<Component> Clone(GameObject* newOwner) const override
    {
        return std::make_unique<MultiParamComponent>(newOwner, intValue, stringValue, floatValue);
    }
};

// Mock component with reference parameter
class RefParamComponent : public Component {
public:
    const std::string& stringRef;
    bool wasAdded = false;

    RefParamComponent(GameObject* owner, const std::string& ref)
        : Component(owner), stringRef(ref) {}

    void OnAdd() override { wasAdded = true; }
    void Update() override {}
    void OnRemove() override {}
    void Disable() override {}
    void Enable() override {}
    std::string ToString() const override { return "RefParamComponent"; }
    std::unique_ptr<Component> Clone(GameObject* newOwner) const override
    {
        return std::make_unique<RefParamComponent>(newOwner, stringRef);
    }
};

// Mock component with move-only parameter
class MoveOnlyParamComponent : public Component {
public:
    std::unique_ptr<int> uniquePtr;
    bool wasAdded = false;

    MoveOnlyParamComponent(GameObject* owner, std::unique_ptr<int> ptr)
        : Component(owner), uniquePtr(std::move(ptr)) {}

    void OnAdd() override { wasAdded = true; }
    void Update() override {}
    void OnRemove() override {}
    void Disable() override {}
    void Enable() override {}
    std::string ToString() const override { return "MoveOnlyParamComponent"; }
    // Cannot clone: holds a move-only unique_ptr with no way to duplicate the value.
    std::unique_ptr<Component> Clone(GameObject* newOwner) const override
    {
        throw std::logic_error("MoveOnlyParamComponent cannot be cloned.");
    }
};

class EmplaceComponentTest : public ::testing::Test {
protected:
    MockLogger logger;
    std::unique_ptr<GameObject> gameObject;

    void SetUp() override {
        // FIX: Pass Vector2 as third argument, then name string (constructor order: logger, id, position, name)
        gameObject = std::make_unique<GameObject>(logger, 1, Transform{}, "TestObject");
    }
};

TEST_F(EmplaceComponentTest, EmplaceSimpleComponent) {
    // Act
    auto* component = gameObject->EmplaceComponent<SimpleComponent>();

    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_TRUE(component->wasAdded);
    EXPECT_FALSE(component->wasUpdated);
    EXPECT_EQ(component->GetOwner(), gameObject.get());
}

TEST_F(EmplaceComponentTest, EmplaceSingleParamComponent) {
    // Arrange
    int testValue = 42;

    // Act
    auto* component = gameObject->EmplaceComponent<SingleParamComponent>(testValue);

    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_TRUE(component->wasAdded);
    EXPECT_EQ(component->value, testValue);
    EXPECT_EQ(component->GetOwner(), gameObject.get());
}

TEST_F(EmplaceComponentTest, EmplaceMultiParamComponent) {
    // Arrange
    int intVal = 123;
    std::string stringVal = "test string";
    float floatVal = 3.14f;

    // Act
    auto* component = gameObject->EmplaceComponent<MultiParamComponent>(
        intVal, stringVal, floatVal);

    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_TRUE(component->wasAdded);
    EXPECT_EQ(component->intValue, intVal);
    EXPECT_EQ(component->stringValue, stringVal);
    EXPECT_FLOAT_EQ(component->floatValue, floatVal);
    EXPECT_EQ(component->GetOwner(), gameObject.get());
}

TEST_F(EmplaceComponentTest, EmplaceWithReferenceParam) {
    // Arrange
    std::string testString = "reference test";

    // Act
    auto* component = gameObject->EmplaceComponent<RefParamComponent>(testString);

    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_TRUE(component->wasAdded);
    EXPECT_EQ(component->stringRef, testString);
    EXPECT_EQ(&component->stringRef, &testString); // Same memory location
    EXPECT_EQ(component->GetOwner(), gameObject.get());
}

TEST_F(EmplaceComponentTest, EmplaceWithMoveOnlyParam) {
    // Arrange
    auto uniqueInt = std::make_unique<int>(99);
    int expectedValue = *uniqueInt;

    // Act
    auto* component = gameObject->EmplaceComponent<MoveOnlyParamComponent>(
        std::move(uniqueInt));

    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_TRUE(component->wasAdded);
    EXPECT_NE(component->uniquePtr, nullptr);
    EXPECT_EQ(*component->uniquePtr, expectedValue);
    EXPECT_EQ(uniqueInt, nullptr); // Original should be moved from
    EXPECT_EQ(component->GetOwner(), gameObject.get());
}

TEST_F(EmplaceComponentTest, EmplaceMultipleComponentsSameTypeThrows) {
    // Assert
    EXPECT_THROW(
        {
            auto* first = gameObject->EmplaceComponent<SingleParamComponent>(10);
            auto* second = gameObject->EmplaceComponent<SingleParamComponent>(20);
        }, 
        DuplicateComponentException);
}

TEST_F(EmplaceComponentTest, EmplaceComponentReturnsCorrectType) {
    // Act
    auto* component = gameObject->EmplaceComponent<MultiParamComponent>(1, "test", 2.0f);

    // Assert
    // Should be able to call methods specific to MultiParamComponent
    EXPECT_EQ(component->intValue, 1);
    EXPECT_EQ(component->stringValue, "test");
    EXPECT_FLOAT_EQ(component->floatValue, 2.0f);
}

TEST_F(EmplaceComponentTest, EmplaceComponentIsRetrievable) {
    // Arrange
    int testValue = 777;

    // Act
    auto* emplaced = gameObject->EmplaceComponent<SingleParamComponent>(testValue);
    auto* retrieved = gameObject->GetComponent<SingleParamComponent>();

    // Assert
    EXPECT_EQ(emplaced, retrieved);
    EXPECT_EQ(retrieved->value, testValue);
}

TEST_F(EmplaceComponentTest, EmplaceWithConstReferenceString) {
    // Arrange
    const std::string constString = "const reference test";

    // Act
    auto* component = gameObject->EmplaceComponent<RefParamComponent>(constString);

    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_EQ(component->stringRef, constString);
}

TEST_F(EmplaceComponentTest, EmplaceWithTemporaryObjects) {
    // Act - using temporary objects as arguments
    auto* component = gameObject->EmplaceComponent<MultiParamComponent>(
        42,                    // temporary int
        std::string("temp"),   // temporary string
        1.5f                   // temporary float
    );

    // Assert
    EXPECT_NE(component, nullptr);
    EXPECT_EQ(component->intValue, 42);
    EXPECT_EQ(component->stringValue, "temp");
    EXPECT_FLOAT_EQ(component->floatValue, 1.5f);
}
