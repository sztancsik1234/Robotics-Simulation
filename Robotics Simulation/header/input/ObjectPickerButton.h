#pragma once
#include "ButtonComponent.h"
#include "core/GameObject.h"
#include <core/Scene.h>

class ObjectPickerButton : public ButtonComponent
{
public:
    ObjectPickerButton(GameObject* owner, 
                       IInputService& inputService,
                       Scene& currentScene,
                       GameObject&& targetObject)
        : ButtonComponent(owner, inputService), currentScene(currentScene), targetObject(std::move(targetObject))
    {}

protected:
    void onClick() override;

    void onHovered() override {}

private:
    Scene& currentScene;
    GameObject targetObject;

};

