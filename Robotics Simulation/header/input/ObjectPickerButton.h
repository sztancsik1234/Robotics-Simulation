#pragma once
#include "ButtonComponent.h"
#include "core/GameObject.h"
#include <core/Scene.h>
#include "core/MessageDispatcher.h"
#include <events/GhostCreatedEvent.h>
#include <events/GhostPlacedEvent.h>

class ObjectPickerButton : public ButtonComponent, public IObserver<GhostCreatedEvent>, public IObserver<GhostPlacedEvent>
{
public:
    ObjectPickerButton(GameObject* owner, 
                       ILogger& Logger,
                       IInputService& inputService,
                       CentralMessageDispatcher& messageDispatcher,
                       std::unique_ptr<Scene>* currentScene,
                       GameObject&& targetObject)
        : ButtonComponent(owner, inputService), Logger(Logger), messageDispatcher(messageDispatcher), currentScene(currentScene), targetObject(std::move(targetObject))
    {
        Logger.Log("ObjectPicker constructed! TargetObject: " + this->targetObject.ToString(), LogLevel::INFO);
        this->targetObject.LogComponents();
    }

    GameObject* GetTargetObject();

    void onNotify(GhostCreatedEvent* e) override;
    void onNotify(GhostPlacedEvent* e) override;
protected:
    void onClick() override;
    void onHovered() override {}

	// Cannot clone: holds a GameObject by value which requires a copy constructor not yet implemented.
    std::unique_ptr<Component> Clone(GameObject* newOwner) const override;

private:
    ILogger& Logger;
    CentralMessageDispatcher& messageDispatcher;

    std::unique_ptr<Scene>* currentScene;
    GameObject targetObject;

    GameObject* currentlyPickedGhostPtr = nullptr;

    void onAdded() override;
    void onRemoved() override;

};

