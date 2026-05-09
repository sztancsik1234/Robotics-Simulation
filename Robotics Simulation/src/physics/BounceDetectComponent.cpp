#include "physics/BounceDetectComponent.h"
#include "core/GameObject.h"
#include <algorithm>

void BounceDetectComponent::OnAdd()
{
	logger.Log(std::format("[BounceDetectComponent] Added to {}", GetOwner()->ToString()), LogLevel::TRACE);
}

void BounceDetectComponent::Update()
{
	GameObject& owner = *GetOwner();
	
	// calculate current position change
	Vector2 currentPosition = owner.GetPosition();
	Vector2 currentPositionChange = currentPosition - previousPosition;

	// get the previous position change and calculate the change as a vector
	Vector2 Acceleration = currentPositionChange - previousPositionChange;

	// get the magnitude of the acceleration vector and compare to threshold
	if (float accelerationMagnitude = std::sqrt(Acceleration.x * Acceleration.x + Acceleration.y * Acceleration.y); 
		accelerationMagnitude >= tresholdAcceleration)
	{
		logger.Log(std::format("[BounceDetectComponent] Bounce detected on {} at Position: {}", owner.ToString(), (std::string)owner.GetPosition()), LogLevel::INFO);
	}

	// update previous position and position change for the next frame
	previousPosition = currentPosition;
	previousPositionChange = currentPositionChange;
}

void BounceDetectComponent::OnRemove()
{}

std::string BounceDetectComponent::ToString() const
{
	return "BounceDetectComponent";
}
