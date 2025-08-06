#include <type_traits>
#include <concepts>
#include "GameObject.h"

template <ComponentDerived T>
Component* GameObject::GetComponent(T cmp)
{
    // iterate through the component list
    for (auto& component : componentList)
    {
        // if the component is of type T, return it
        if (typeid(component) == typeid(cmp))
        {
            return &component;
        }
    }
    throw ComponentNotFoundException();
}