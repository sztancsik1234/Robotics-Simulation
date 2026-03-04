#pragma once
#include "core/GameObject.h"
#include "ICameraRenderer.h"
#include "RenderComponent.h"
#include "util/ILogger.h"

/// <summary>
/// A mock class to render a circle on the screen.
/// </summary>
class CircleRendererComponent : public RenderComponent
{
public:
	CircleRendererComponent(GameObject* owner, ICameraRenderer& camera, ILogger& logger) :
		RenderComponent(owner, logger, camera)
	{}

	void Update() override;
};