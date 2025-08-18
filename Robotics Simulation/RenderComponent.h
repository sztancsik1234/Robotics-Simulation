#pragma once
#include "Component.h"
#include "IRenderer.h"
#include <memory>

// TODO: INCOMPLETE
class RenderComponent final : public Component
{
	public:
	/// <summary>
	/// Constructs a RenderComponent and associates it with a given GameObject.
	/// </summary>
	/// <param name="owner">Pointer to the GameObject that owns this Component.</param>
	RenderComponent(IRenderer* renderer, GameObject* owner) : Component(owner), Renderer(*renderer) {}
	/// <summary>
	/// Called when the component is added to a GameObject. Initializes rendering resources.
	/// </summary>
	void OnAdd() override;
	/// <summary>
	/// Updates the rendering state. This method can be empty if no updates are needed.
	/// </summary>
	void Update() override;
	/// <summary>
	/// Called when the component is removed from a GameObject. Cleans up rendering resources.
	/// </summary>
	void OnRemove() override;

private:
	IRenderer& Renderer; // Reference to the renderer for drawing operations
};

