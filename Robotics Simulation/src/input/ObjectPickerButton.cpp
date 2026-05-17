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
	// BUG! When the button is clicked again, the clickEvent is processed first, always setting the currentlyPickedGhostPtr as nullptr first. 
	// FIXED: GhostComponent update is now delayed 1 frame
	if (currentlyPickedGhostPtr != nullptr)
	{
		//return;
		currentScene->get()->RemoveGameObject(currentlyPickedGhostPtr);
	}

	Logger.Log("Spawning object:", LogLevel::INFO);
	targetObject.LogComponents();
	GameObject* placedgo = currentScene->get()->AddGameObject(targetObject);

	// Broadcast GhostCreatedEvent
	GhostCreatedEvent createdEvt(placedgo);
	messageDispatcher.ghostCreatedEventBroadcast.broadcast(&createdEvt);
}

std::unique_ptr<Component> ObjectPickerButton::Clone(GameObject* newOwner) const
{
	throw std::logic_error("ObjectPickerButton cannot be cloned: depends on GameObject copy constructor.");
}

void ObjectPickerButton::onAdded()
{
	messageDispatcher.ghostCreatedEventBroadcast.subscribe(this);
	messageDispatcher.ghostPlacedEventBroadcast.subscribe(this);
}

void ObjectPickerButton::onRemoved()
{
	messageDispatcher.ghostPlacedEventBroadcast.unsubscribe(this);
	messageDispatcher.ghostCreatedEventBroadcast.unsubscribe(this);
}

void ObjectPickerButton::onNotify(GhostPlacedEvent* e)
{
	currentlyPickedGhostPtr = nullptr;
}

void ObjectPickerButton::onNotify(GhostCreatedEvent* e)
{
	currentlyPickedGhostPtr = e->createdGhost;
}
