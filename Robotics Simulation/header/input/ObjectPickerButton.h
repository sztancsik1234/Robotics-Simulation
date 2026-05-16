#pragma once
#include "ButtonComponent.h"
#include "core/GameObject.h"
#include <core/Scene.h>

class ObjectPickerButton : public ButtonComponent
{
public:
    ObjectPickerButton(GameObject* owner, 
                       ILogger& Logger,
                       IInputService& inputService,
                       std::unique_ptr<Scene>* currentScene,
                       GameObject&& targetObject)
        : ButtonComponent(owner, inputService), Logger(Logger), currentScene(currentScene), targetObject(std::move(targetObject))
    {
        Logger.Log("ObjectPicker constructed! TargetObject: " + this->targetObject.ToString(), LogLevel::INFO);
        this->targetObject.LogComponents();
    }

    GameObject* GetTargetObject();

protected:
    void onClick() override;
    void onHovered() override {}

	// Cannot clone: holds a GameObject by value which requires a copy constructor not yet implemented.
    std::unique_ptr<Component> Clone(GameObject* newOwner) const override;

private:
    ILogger& Logger;

    std::unique_ptr<Scene>* currentScene;
    GameObject targetObject;

};

