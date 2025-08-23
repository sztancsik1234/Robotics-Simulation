#pragma once
#include "core/Component.h"
#include "graphics/IRenderer.h"
#include "util/ILogger.h"

class RenderComponent : public Component
{
public:
	/// <summary>
	/// Constructs a RenderComponent and associates it with a given GameObject.
	/// </summary>
	/// <param name="owner">Pointer to the GameObject that owns this Component.</param>
	RenderComponent(GameObject* owner, ILogger& logger) :	// IDrawableRenderer doesn't allow to pass ISpriteRenderer or IPrimitiveRenderer
		Component(owner), Logger(logger)							// Copilot recommends just not passing a renderer, just in the derived classes.
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

protected:
	ILogger& Logger;
};

