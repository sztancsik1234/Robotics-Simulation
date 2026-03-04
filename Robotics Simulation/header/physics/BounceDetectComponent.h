#pragma once

#include "core/Component.h"
#include "core/Vector2.h"
#include "util/ILogger.h"
#include "core/GameObject.h"
#include <string>

/// <summary>
/// A debug component, that calculates the direction of movement and logs a message if a sufficently big change is detected.
/// </summary>

class BounceDetectComponent : public Component
{
public:
	explicit BounceDetectComponent(GameObject* owner, ILogger& logger, float tresholdAcceleration)
		: Component(owner), logger(logger), tresholdAcceleration(tresholdAcceleration), previousPosition { 0.f, 0.f }
	{}

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;
	std::string ToString() const override;

private:
	ILogger& logger;
	float tresholdAcceleration;
	Vector2 previousPositionChange;
	Vector2 previousPosition;
};