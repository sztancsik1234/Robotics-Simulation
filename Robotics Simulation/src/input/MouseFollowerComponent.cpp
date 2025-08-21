#include "input/MouseFollowerComponent.h"

void MouseFollowerComponent::OnAdd()
{
	// no logic needed
}

void MouseFollowerComponent::Update()
{
	// Get the current mouse position
	Vector2 mousePosition = InputService.GetMousePosition();
	
	// Update the position of the GameObject to follow the mouse
	GetOwner()->SetPosition(mousePosition);
}

void MouseFollowerComponent::OnRemove()
{
	// Cleanup logic if needed
	// This could include resetting the position or state of the GameObject
}
