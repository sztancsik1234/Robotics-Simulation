#pragma once
#include "core/Component.h"
#include "util/ILogger.h"
#include "graphics/IRenderer.h"
#include "graphics/Camera.h"


class RenderComponent : public Component
{
public:
	/// <summary>
	/// Constructs a RenderComponent and associates it with a given GameObject.
	/// </summary>
	/// <param name="owner">Pointer to the GameObject that owns this Component.</param>
	RenderComponent(GameObject* owner, ILogger& logger, Camera& camera) :
		Component(owner),
		Logger(logger),
		camera(camera)
	{
	}

	~RenderComponent() override = default;

	/// <summary>
	/// Called when the component is added to a GameObject. Initializes rendering resources.
	/// </summary>
	void OnAdd() override;

	/// <summary>
	/// Draws the component's visual representation each frame.
	/// </summary>
	void Update() override;

	/// <summary>
	/// Called when the component is removed from a GameObject. Cleans up rendering resources.
	/// </summary>
	void OnRemove() override;

protected:
	ILogger& Logger;
	Camera& camera;
};