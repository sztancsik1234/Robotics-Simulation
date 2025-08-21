#include "graphics/CircleRenderer.h"

void CircleRenderer::Update()
{
    // Draw a circle at the GameObject's position with a fixed radius  
	GameObject const& owner = *GetOwner();
    Vector2 position = owner.GetPosition();
    Renderer.DrawCircle(position, 50.0f); // Example radius of 50.0f
    Logger.Log("In CircleRenderer::Update Gameobject.position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")", LogLevel::TRACE);
}
