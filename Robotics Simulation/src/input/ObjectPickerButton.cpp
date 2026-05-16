#include "input/ObjectPickerButton.h"
#include <iostream>

GameObject* ObjectPickerButton::GetTargetObject()
{
	return &targetObject;
}

void ObjectPickerButton::onClick()
{
	if (isDisabled())
	{
		return;
	}
	Logger.Log("Spawning object:", LogLevel::INFO);
	targetObject.LogComponents();
	currentScene->get()->AddGameObject(targetObject);
}

std::unique_ptr<Component> ObjectPickerButton::Clone(GameObject* newOwner) const
{
	throw std::logic_error("ObjectPickerButton cannot be cloned: depends on GameObject copy constructor.");
}
