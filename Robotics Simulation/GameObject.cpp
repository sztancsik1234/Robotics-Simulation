#include <type_traits>
#include <concepts>
#include "GameObject.h"
#include "Component.h"



// thoughts on implementing aan EntityComponentSystem (ECS) pattern
// May be overkill for this project, but could be useful if it grows

GameObject::~GameObject()
{
    // iterate through the component list and call OnRemove for each component
    for (auto const& component : componentList)
    {
        component->OnRemove();
    }
    
    // clear the component list
	componentList.clear();
	Logger.Log("GameObject with ID " + std::to_string(id) + " destroyed.");
}

GameObject::GameObject(GameObject&& other) noexcept
    : id(other.id),
      name(std::move(other.name)),
      Position(other.Position),
      componentList(std::move(other.componentList)),
	Logger(other.Logger)
{
    // Reset the moved-from object to a valid state
    other.id = 0;
    other.Position = Vector2();
    other.name = "Unnamed";
    // componentList is already moved from (empty)
    for (auto& component : componentList) {
        component->SetOwner(this); // Set the owner of the components to this GameObject
        Logger.Log("Component moved to new GameObject with ID " + std::to_string(id), LogLevel::INFO);
    }
}

void GameObject::SetPosition(const Vector2& position)
{
    Position = position;
	Logger.Log("GameObject position set to: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")", LogLevel::TRACE);
}

void GameObject::AddComponent(std::unique_ptr<Component> component)
{
	Logger.Log("Component added without emplace.", LogLevel::WARNING);
	component->OnAdd(); // call OnAdd for the component
    // add the component to the list
	componentList.push_front(std::move(component));
}

void GameObject::Update()
{
    Logger.Log("In GameObject::Update:\tGameobject.position: (" + std::to_string(GetPosition().x) + ", " + std::to_string(GetPosition().y) + ")", LogLevel::TRACE);

    // iterate through the component list and call Update for each component
    for (auto const& component : componentList)
    {
        component->Update();
    }
}
