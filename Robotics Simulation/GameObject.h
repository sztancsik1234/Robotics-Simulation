#pragma once
#include <type_traits>
#include <concepts>
#include "Vector2.h"
#include <forward_list>


class Component
{
	int asd;
};

template<typename T>
concept ComponentDerived = std::is_base_of<Component, T>::value;

class GameObject
{
public:
	Vector2 Position;
	
	template <ComponentDerived T>
	Component* GetComponent(T cmp);
protected:
	std::forward_list<Component> componentList;
};

class ComponentNotFoundException : public std::runtime_error
{
	public:
	ComponentNotFoundException(const std::string& message)
		: std::runtime_error(message) {}
};
	
