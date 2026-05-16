#include "core/Component.h"
#include "core/GameObject.h"
#include "util/Exceptions.h"
#include "core/Angle.h"
#include "core/Transform.h"
#include "core/Vector2.h"
#include <format>
#include <memory>
#include <string>
#include "util/ILogger.h"
#include <utility>



// thoughts on implementing an EntityComponentSystem (ECS) pattern:
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
	Logger.Log("[GameObject] GameObject with ID " + std::to_string(id) + " destroyed.");
}

GameObject::GameObject(const GameObject& other) :
	id(other.id),
	name(other.name),
	transform(other.transform),
	anchor(other.anchor),
	Logger(other.Logger)
{
	for (const auto& component : other.componentList)
	{
		auto cloned = component->Clone(this);
		cloned->OnAdd();
		componentList.push_back(std::move(cloned));
	}
}

GameObject::GameObject(GameObject&& other) noexcept
	: id(other.id),
	name(std::move(other.name)),
	transform(other.transform),
	anchor(other.anchor),
	componentList(std::move(other.componentList)),
	Logger(other.Logger)
{
	// Reset the moved-from object to a valid state
	other.id = 0;
	other.transform = Transform();
	other.anchor = { 0.5f, 0.5f };
	other.name = "Unnamed";
	// componentList is already moved from (empty)
	for (auto& component : componentList)
	{
		component->SetOwner(this); // Set the owner of the components to this GameObject
		Logger.Log("[GameObject] Component moved to new GameObject with ID " + std::to_string(id), LogLevel::INFO);
	}
}

void GameObject::SetPosition(const Vector2& position)
{
	transform.position = position;
	Logger.Log(std::format("[GameObject] GameObject position set to: ({}, {})", std::to_string(position.x), std::to_string(position.y)), LogLevel::TRACE);
}

void GameObject::SetRotation(const Radian& rotation)
{
	transform.rotation = rotation;
	Logger.Log("[GameObject] GameObject rotation set to: " + std::to_string((float)rotation) + " radians", LogLevel::TRACE);
}

void GameObject::SetTransform(const Transform& newTransform)
{
	transform = newTransform;
	Logger.Log("[GameObject] GameObject transform set. Position: (" + std::to_string(transform.position.x) + ", " + std::to_string(transform.position.y) + "), Rotation: " + std::to_string((float)transform.rotation) + " radians", LogLevel::TRACE);
}

void GameObject::SetAnchor(const Vector2& newAnchor)
{
	anchor = newAnchor;
	Logger.Log(std::format("[GameObject] GameObject anchor set to: ({}, {})", std::to_string(anchor.x), std::to_string(anchor.y)), LogLevel::TRACE);
}

void GameObject::AddComponent(std::unique_ptr<Component> component)
{
	component->OnAdd(); // call OnAdd for the component
	// add the component to the list
	componentList.push_front(std::move(component));
	Logger.Log("[GameObject] Component added without emplace.", LogLevel::WARNING);
}

void GameObject::RemoveComponent(Component const* component)
{
	for (auto it = componentList.begin(); it != componentList.end(); ++it)
	{
		if (it->get() == component)
		{
			(*it)->OnRemove();
			Logger.Log("Component removed.", LogLevel::INFO);
			componentsMarkedForRemoval.push_back(it);
			return;
		}
	}
	Logger.Log("Attempted to remove non-existent component. Operation aborted.", LogLevel::WARNING);
	throw ComponentNotFoundException();
}



void GameObject::Update()
{
	Logger.Log(std::format("[GameObject] '{}': position: ({}, {})", name, std::to_string(GetPosition().x), std::to_string(GetPosition().y)), LogLevel::TRACE);

	// iterate through the component list and call Update for each component
	for (auto const& component : componentList)
	{
		component->Update();
	}

	DeleteComponents();
}

/// <summary>
/// Frees removed component memory
/// </summary>
void GameObject::DeleteComponents()
{
	for (auto compIt : componentsMarkedForRemoval)
	{
		Logger.Log(compIt->get()->ToString() + " freed!", LogLevel::INFO);
		componentList.erase(compIt);
	}

	componentsMarkedForRemoval.clear();
}

std::string GameObject::ToString(bool components) const
{
	std::string result = std::format("<Gameobject> id={}, name={}", id, name);
	if (components)
	{
		for (auto const& component : componentList)
			result += "\n\t - " + component->ToString();
		result += "\n";
	}
	return result;
}

void GameObject::LogComponents() const
{
	std::string debugStr = std::format("[GameObject] Components of GameObject id={}, name='{}':", id, name);
	for (auto const& component : componentList)
	{
		debugStr += "\n - " + component->ToString();
	}
	Logger.Log(debugStr, LogLevel::INFO);
}
