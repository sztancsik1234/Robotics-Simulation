#pragma once
#include "core/Component.h"
#include "physics/IPhysicsEngine.h"
#include "util/ILogger.h"
#include "core/GameObject.h"
#include "core/Vector2.h"
#include <string>

// Inherits the owner's transform to create a static box in the physics engine.
class BallPhysicsComponent final : public Component
{
public:
	BallPhysicsComponent(GameObject* owner, ILogger& logger, IBodyManager& physicsEngine) :
		Component(owner),
		logger(logger),
		PhysicsEngine(physicsEngine)
	{}

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;
	void Disable() override;
	void Enable() override;
	std::string ToString() const override;

	std::unique_ptr<Component> Clone(GameObject* newOwner) const override;

private:
	ILogger& logger;
	IBodyManager& PhysicsEngine;
	BodyId bodyId = 0;
	// Normalized offset from the top-left corned of the box.
	// Unused for now. If a body is needed with offset, this can be used.
	// When used, update needs to be modified to set the body's position based on the owner's position and the anchor.
	Vector2 anchor = { 0.5f, 0.5f };
};