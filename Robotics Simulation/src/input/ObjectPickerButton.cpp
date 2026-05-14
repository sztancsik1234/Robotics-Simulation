#include "input/ObjectPickerButton.h"
#include <iostream>

void ObjectPickerButton::onClick()
{
	currentScene.AddGameObject(targetObject);
}
