#include "input/MouseFollowerComponent.h"

void MouseFollowerComponent::OnAdd()
{
	// no logic needed
}

void MouseFollowerComponent::Update()
{
	// Get the current mouse position
	Vector2 mousePosition = InputService.GetMousePosition();

	//worldposition:
	Vector2 worldMousePosition = camera.PixelToWorldPos(mousePosition);
	
	// Update the position of the GameObject to follow the mouse
	GetOwner()->SetPosition(worldMousePosition);
}

void MouseFollowerComponent::OnRemove()
{
	// Cleanup logic if needed
	// This could include resetting the position or state of the GameObject
}

std::string MouseFollowerComponent::ToString() const
{
	return std::format("<MouseFollowerComponent> owner={}", GetOwner()->GetId());
}
