#pragma once
#include <type_traits>
#include <concepts>
#include "Vector2.h"
#include <forward_list>
#include <memory>



class Component
{
	int asd;
};

template<typename T>
concept ComponentDerived = std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value;

class GameObject
{
public:
	virtual ~GameObject() = default;

	Vector2 GetPosition() const { return Position; }
	void SetPosition(const Vector2& position) { Position = position; }
	template <ComponentDerived T>
	std::unique_ptr<T> GetComponent();

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
	
