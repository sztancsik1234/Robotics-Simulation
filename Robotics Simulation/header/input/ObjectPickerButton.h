#pragma once
#include "ButtonComponent.h"
#include "core/GameObject.h"
#include <core/Scene.h>

class ObjectPickerButton : public ButtonComponent
{
public:
    ObjectPickerButton(GameObject* owner, IInputService& inputService, Scene& activeScene, GameObject&& targetObject)
        : ButtonComponent(owner, inputService), currentScene(activeScene), targetObject(std::move(targetObject))
    {}

protected:
    void onClick() override
    {
        // Handle object picking logic here
    }

    void onHovered() override
    {
        // Handle hover logic here
    }

private:
	Scene& currentScene;
    GameObject targetObject;

};

