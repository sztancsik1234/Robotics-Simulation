#pragma once
#include "RenderComponent.h"
#include "core/GameObject.h"
#include "core/ComponentDTOs.h"

/// <summary>
/// A mock class to render a circle on the screen.
/// </summary>
class CircleRenderer : public RenderComponent
{
public:
	// Refactored constructors to take Camera instead of primitive renderer
	CircleRenderer(GameObject* owner, Camera& camera, ILogger& logger) :
		RenderComponent(owner, logger, camera)
	{
	}

	void Update() override;
};