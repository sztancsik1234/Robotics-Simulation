#pragma once
#include "Angle.h"
#include "Component.h"
#include "Transform.h"
#include "util/ILogger.h"
#include "Vector2.h"
#include <list>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>

class ComponentNotFoundException;
class DuplicateComponentException;

template<typename T>
concept ComponentDerived = std::is_base_of_v<Component, T>;

class GameObject
{
public:
	// TODO: Pass transform by const reference
	explicit GameObject(ILogger& logger, int id = 0, Transform transform = Transform {}, const std::string& name = "Unnamed", Vector2 anchor = { 0.5f, 0.5f }) :
		id(id), name(name), transform(transform), anchor(anchor), Logger(logger)
	{}
	~GameObject();
	GameObject(GameObject&& other) noexcept;

	Vector2 GetPosition() const { return transform.position; }
	Radian GetRotation() const { return transform.rotation; }
	Transform GetTransform() const { return transform; }
	Vector2 GetAnchor() const { return anchor; }
	void SetPosition(const Vector2& position);
	void SetRotation(const Radian& rotation);
	void SetTransform(const Transform& transform);
	void SetAnchor(const Vector2& newAnchor);

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

	template <ComponentDerived T>
	bool HasComponent() const
	{
		for (const auto& component : componentList)
		{
			if (dynamic_cast<T*>(component.get()) != nullptr)
			{
				return true;
			}
		}
		return false;
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
		requires requires(GameObject* owner, Args&&... args)
	{
		ComponentType { owner, std::forward<Args>(args)... };		// TODO: check out std::is_constructible_v
	}
	ComponentType* EmplaceComponent(Args && ...args)
	{
		if (HasComponent<ComponentType>())
		{
			Logger.Log("Attempted to add duplicate component of type " + std::string(typeid(ComponentType).name()) + ". Operation aborted.", LogLevel::WARNING);
			throw DuplicateComponentException();
		}
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

	template<ComponentDerived ComponentType>
	void RemoveComponent()
	{
		for (auto it = componentList.begin(); it != componentList.end(); ++it)
		{
			if (dynamic_cast<ComponentType*>(it->get()) != nullptr)
			{
				(*it)->OnRemove();
				componentList.erase(it);
				Logger.Log("Component of type " + std::string(typeid(ComponentType).name()) + " removed.", LogLevel::INFO);
				return;
			}
		}
		Logger.Log("Attempted to remove non-existent component of type " + std::string(typeid(ComponentType).name()) + ". Operation aborted.", LogLevel::WARNING);
		throw ComponentNotFoundException();
	}

	void RemoveComponent(Component* component);

	/// <summary>
	/// Updates the game object by calling Update on each component. Called from the Game class. If a derived class wants to implement it's own update logic, it should just add a component that implements it.
	/// </summary>
	void Update();

	// tostring method for debugging
	std::string ToString(bool components = false) const;

	void LogComponents() const;
private:
	int id;
	std::string name;
	Transform transform;
	Vector2 anchor;
	std::list<std::unique_ptr<Component>> componentList;

	ILogger& Logger;
};

class ComponentNotFoundException : public std::runtime_error
{
public:
	ComponentNotFoundException()
		: std::runtime_error("Component not found!") {}
};

class DuplicateComponentException : public std::runtime_error
{
public:
	DuplicateComponentException()
		: std::runtime_error("Added component already exists!") {}
};
