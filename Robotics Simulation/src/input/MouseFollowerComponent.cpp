#include "input/MouseFollowerComponent.h"

void MouseFollowerComponent::OnAdd()
{
	// no logic needed
}

void MouseFollowerComponent::Update()
{
	if (disabled)
	{
		return;
	}
	// Get the current mouse position
	Vector2 mousePosition = InputService.GetMousePosition();

	//worldposition:
	Vector2 worldMousePosition = viewPort.PixelToWorldPos(mousePosition);
	
	// Update the position of the GameObject to follow the mouse
	GetOwner()->SetPosition(worldMousePosition);
}

void MouseFollowerComponent::OnRemove()
{
	// Cleanup logic if needed
	// This could include resetting the position or state of the GameObject
}

void MouseFollowerComponent::Disable()
{
	disabled = true;
}

void MouseFollowerComponent::Enable()
{
	disabled = false;
}

std::string MouseFollowerComponent::ToString() const
{
	return std::format("<MouseFollowerComponent>");
}

std::unique_ptr<Component> MouseFollowerComponent::Clone(GameObject* newOwner) const
{
	return std::make_unique<MouseFollowerComponent>(newOwner, viewPort, InputService);
}
