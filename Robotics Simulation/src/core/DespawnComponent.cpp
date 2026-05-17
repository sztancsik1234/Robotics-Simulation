#include "core/DespawnComponent.h"
#include "core/GameObject.h"
#include <format>

void DespawnComponent::OnAdd()
{
	logger.Log(std::format("[DespawnComponent] Added to {}. Despawn threshold Y={}",
		GetOwner()->ToString(), despawnY), LogLevel::TRACE);
	enabled = true;
}

void DespawnComponent::Update()
{
	if (!enabled)
		return;

	const float currentY = GetOwner()->GetPosition().y;
	if (currentY < despawnY)
	{
		logger.Log(std::format("[DespawnComponent] {} fell below Y threshold ({} > {}). Despawning.",
			GetOwner()->ToString(), currentY, despawnY), LogLevel::INFO);

		scene->get()->RemoveGameObject(GetOwner());
	}
}

void DespawnComponent::OnRemove()
{
	logger.Log(std::format("[DespawnComponent] Removed from {}.", GetOwner()->ToString()), LogLevel::TRACE);
}

void DespawnComponent::Disable()
{
	enabled = false;
	logger.Log("[DespawnComponent] Disabled.", LogLevel::TRACE);
}

void DespawnComponent::Enable()
{
	enabled = true;
	logger.Log("[DespawnComponent] Enabled.", LogLevel::TRACE);
}

std::string DespawnComponent::ToString() const
{
	return std::format("<DespawnComponent despawnY={}>", despawnY);
}

std::unique_ptr<Component> DespawnComponent::Clone(GameObject* newOwner) const
{
	return std::make_unique<DespawnComponent>(newOwner, logger, scene, despawnY);
}
