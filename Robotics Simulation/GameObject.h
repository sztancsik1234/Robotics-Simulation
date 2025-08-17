#pragma once
#include <type_traits>
#include <concepts>
#include "Vector2.h"
#include <forward_list>
#include <memory>

class Component;

template<typename T>
concept ComponentDerived = std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value;

class GameObject
{
public:
	virtual ~GameObject();

	Vector2 GetPosition() const { return Position; }
	void SetPosition(const Vector2& position) { Position = position; }
	template <ComponentDerived T>
	std::unique_ptr<T> GetComponent();
	void AddComponent(std::unique_ptr<Component> component);

private:
	Vector2 Position;
	
	std::forward_list<std::unique_ptr<Component>> componentList;
};

class ComponentNotFoundException : public std::runtime_error
{
	public:
	ComponentNotFoundException(const std::string& message)
		: std::runtime_error(message) {}
};
	
