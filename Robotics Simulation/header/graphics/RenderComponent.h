#pragma once
#include "core/Component.h"
#include "graphics/ICameraRenderer.h"
#include "util/ILogger.h"
#include "core/GameObject.h"
#include <string>


class RenderComponent : public Component
{
public:
	/// <summary>
	/// Constructs a RenderComponent and associates it with a given GameObject.
	/// </summary>
	/// <param name="owner">Pointer to the GameObject that owns this Component.</param>
	RenderComponent(GameObject* owner, ILogger& logger, ICameraRenderer& camera) :
		Component(owner),
		Logger(logger),
		camera(camera)
	{}

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

	std::string ToString() const override;

protected:
	ILogger& Logger;
	ICameraRenderer& camera;
};