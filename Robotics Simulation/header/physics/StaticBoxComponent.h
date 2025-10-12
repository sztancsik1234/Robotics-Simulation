#pragma once
#include "core/Component.h"
#include "physics/IPhysicsEngine.h"
#include "core/Transform.h"

// Created with the owner's position at the center of the box
class StaticBoxComponent : public Component
{
public:
	StaticBoxComponent(GameObject* owner, IBodyManager& physicsEngine, float width, float height) :
		Component(owner),
		PhysicsEngine(physicsEngine),
		initialWidth(width),
		initialHeight(height)
	{}

	void OnAdd() override;
	void Update() override;
	void OnRemove() override;

private:
	IBodyManager& PhysicsEngine;
	BodyId BodyId = 0;
	float initialWidth;
	float initialHeight;
};