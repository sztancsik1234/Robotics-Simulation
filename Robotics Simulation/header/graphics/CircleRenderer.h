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
	// Existing constructor
	CircleRenderer(GameObject* owner, IPrimitiveRenderer& renderer, ILogger& logger) :
		RenderComponent(owner, logger),
		Renderer(renderer)
	{}

	// New DTO-based constructor (data currently empty, kept for consistency)
	CircleRenderer(GameObject* owner, IPrimitiveRenderer& renderer, ILogger& logger,
		const CircleRendererDTO) :
		RenderComponent(owner, logger),
		Renderer(renderer)
	{}

	void Update() override;

private:
	IPrimitiveRenderer& Renderer;
};
