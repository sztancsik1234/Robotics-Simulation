#pragma once
#include "core/Component.h"
#include "core/Scene.h"
#include "util/ILogger.h"
#include <string>
#include <memory>

/// <summary>
/// Removes the owning GameObject from the Scene when its Y position
/// exceeds a configured threshold (i.e. the object has fallen below
/// a certain height).
/// </summary>
class DespawnComponent final : public Component
{
public:
	/// <param name="owner">The GameObject that owns this component.</param>
	/// <param name="logger">Logger used for diagnostics.</param>
	/// <param name="scene">The scene the owner belongs to.</param>
	/// <param name="despawnY">
	/// Y-coordinate threshold. The object is despawned when its
	/// world-space Y position is greater than this value.
	/// (In screen / Box2D-style coordinates Y increases downwards.)
	/// </param>
	DespawnComponent(GameObject* owner, ILogger& logger, std::unique_ptr<Scene>* scene, float despawnY)
		: Component(owner), logger(logger), scene(scene), despawnY(despawnY)
	{}

	~DespawnComponent() override = default;

	void OnAdd()    override;
	void Update()   override;
	void OnRemove() override;
	void Disable()  override;
	void Enable()   override;

	std::string ToString() const override;

	std::unique_ptr<Component> Clone(GameObject* newOwner) const override;

private:
	ILogger& logger;
	std::unique_ptr<Scene>*   scene;
	float    despawnY;
};
