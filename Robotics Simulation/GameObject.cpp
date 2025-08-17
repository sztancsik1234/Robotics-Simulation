#include <type_traits>
#include <concepts>
#include "GameObject.h"
#include "Component.h"

template <ComponentDerived T>
std::unique_ptr<T> GameObject::GetComponent()
{
    // iterate through the component list
    for (auto& component : componentList)
    {
        // if the component is of type T, return it
        // TODO: typeid is not the best for matching type if the class is not virtual
		// trying to cast component to T returns nullptr if the type does not match. Exploit this for type checking
        if (typeid(*component) == typeid(T))
        {
            return &component;
        }
    }
    throw ComponentNotFoundException();
}

// thoughts on implementing aan EntityComponentSystem (ECS) pattern
// May be overkill for this project, but could be useful if it grows

GameObject::~GameObject()
{
    // iterate through the component list and call OnRemove for each component
    for (auto& component : componentList)
    {
        component->OnRemove();
    }
    
    // clear the component list
	componentList.clear();
}

void GameObject::AddComponent(std::unique_ptr<Component> component)
{
	component->OnAdd(); // call OnAdd for the component
    // add the component to the list
	componentList.push_front(std::move(component));

}
