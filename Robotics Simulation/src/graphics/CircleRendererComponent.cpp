#include "graphics/CircleRendererComponent.h"

void CircleRendererComponent::Update()
{
	// Draw a circle at the GameObject's position with a fixed radius (world-units)
	GameObject& owner = *GetOwner();
	Vector2 position = owner.GetPosition();
	camera.DrawCircle(position, 10.f); // Example world radius of 50.0f
	Logger.Log(std::format("[CircleRendererComponent] Circle drawn at: ({}, {})", position.x, position.y), LogLevel::TRACE);
}

std::unique_ptr<Component> CircleRendererComponent::Clone(GameObject* newOwner) const
{
	return std::make_unique<CircleRendererComponent>(newOwner, camera, Logger);
}
