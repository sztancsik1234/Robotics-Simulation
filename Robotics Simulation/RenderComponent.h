#pragma once
#include "Component.h"
#include "IRenderer.h"
#include "ILogger.h"
#include <memory>

class RenderComponent : public Component
{
public:
	~RenderComponent() override = default;

	/// <summary>
	/// Constructs a RenderComponent and associates it with a given GameObject.
	/// </summary>
	/// <param name="owner">Pointer to the GameObject that owns this Component.</param>
	RenderComponent(GameObject* owner, IDrawableRenderer& renderer, ILogger& logger) : Component(owner), Renderer(renderer), Logger(logger) {}
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
	/// <summary>
	/// stores a reference to the renderer used for drawing operations. Also stores texture and sprite information if needed.
	/// </summary>
	IDrawableRenderer& Renderer; // Reference to the renderer for drawing operations
	ILogger& Logger;
};

