#include "graphics/RenderComponent.h"
#include "core/GameObject.h"
#include "util/Exceptions.h"

void RenderComponent::OnAdd()
{
	Logger.Log("[RenderComponent] Added to GameObject: " + GetOwner()->GetName(), LogLevel::INFO);
}

void RenderComponent::Update()
{
	// This method should be overridden by derived classes to implement specific rendering logic.
}

void RenderComponent::OnRemove()
{
	Logger.Log("[RenderComponent] Removed from GameObject.", LogLevel::INFO);
}

std::string RenderComponent::ToString() const
{
	return std::format("<RenderComponent>");
}