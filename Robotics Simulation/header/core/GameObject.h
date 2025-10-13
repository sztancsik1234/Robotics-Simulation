#pragma once
#include <type_traits>
#include <concepts>
#include "Transform.h"
#include <forward_list>
#include <memory>
#include "util/ILogger.h"
#include "util/Exceptions.h"
#include "Component.h"

class Component;
class ComponentNotFoundException;

template<typename T>
concept ComponentDerived = std::is_base_of_v<Component, T>;

class GameObject
{
public:
	// TODO: Pass transform by const reference
	explicit GameObject(ILogger& logger, int id = 0, Transform transform = Transform{}, const std::string& name = "Unnamed") :
		id(id), name(name), transform(transform), Logger(logger) {}
	~GameObject();
	GameObject(GameObject&& other) noexcept;

	Vector2 GetPosition() const		{ return transform.position; }
	Radian GetRotation() const		{ return transform.rotation; }
	Transform GetTransform() const	{ return transform; }
	void SetPosition(const Vector2& position);
	void SetRotation(const Radian& rotation);
	void SetTransform(const Transform& transform);

	template <ComponentDerived T>
	T* GetComponent()
	{
		// iterate through the component list
		for (auto& component : componentList)
		{
			// if the component is of type T, return a raw pointer to it
			auto derivedComponent = dynamic_cast<T*>(component.get());
			if (derivedComponent != nullptr)
			{
				return derivedComponent;
			}
		}
		throw ComponentNotFoundException();
	}

	int GetId() const { return id; }
	std::string GetName() const { return name; }

	/// <summary>
	/// Adds a component to the gameobject.
	/// </summary>
	/// <typeparam name="...Args"></typeparam>
	/// <typeparam name="ComponentType"></typeparam>
	/// <param name="...args">Arguments for the constructor of the component, except the pointer to owner, which is passed automatically</param>
	/// <returns>Raw pointer to the component created</returns>
	template<ComponentDerived ComponentType, typename ...Args>
	ComponentType* EmplaceComponent(Args && ...args)
	{
		auto component = std::make_unique<ComponentType>(this, std::forward<Args>(args)...);
		ComponentType* rawPtr = component.get();
		component->OnAdd();
		componentList.push_front(std::move(component));
		Logger.Log("Component added with emplace.", LogLevel::INFO);
		return rawPtr;
	}
	
	/// <summary>
	/// Adds a component to the gameobject. Avoid using this component. EmplaceComponent is preferred.
	/// </summary>
	/// <param name="component">A unique pointer to the component to be added.</param>
	void AddComponent(std::unique_ptr<Component> component);

	void RemoveComponent(Component* component);
	
	/// <summary>
	/// Updates the game object by calling Update on each component. Called from the Game class. If a derived class wants to implement it's own update logic, it should just add a component that implements it.
	/// </summary>
	void Update();

	// tostring method for debugging
	std::string ToString() const;
private:
	int id;
	std::string name;
	Transform transform;
	std::forward_list<std::unique_ptr<Component>> componentList;

	ILogger& Logger;
};

class ComponentNotFoundException : public std::runtime_error
{
	public:
	ComponentNotFoundException()
		: std::runtime_error("Component not found!") {}
};
