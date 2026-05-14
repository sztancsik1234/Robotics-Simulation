#pragma once
#include "core/Component.h"
#include "core/Transform.h"
#include "physics/IPhysicsEngine.h"
#include "util/ILogger.h"

// Inherits the owner's transform to create a static box in the physics engine.
class StaticBoxComponent final : public Component
{
public:
	StaticBoxComponent(GameObject* owner, ILogger& logger, IBodyManager& physicsEngine) :
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

private:
	ILogger& logger;
	IBodyManager& PhysicsEngine;
	BodyId BodyId = 0;
	// Normalized offset from the top-left corned of the box.
	// Unused for now. If a body is needed with offset, this can be used.
	// When used, update needs to be modified to set the body's position based on the owner's position and the anchor.
	Vector2 anchor = { 0.5f, 0.5f };
};