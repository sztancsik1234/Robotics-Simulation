#include "input/MouseClickLoggerComponent.h"

void MouseClickLoggerComponent::onNotify(ClickEvent* event)
{
	logger.Log(std::format("[MouseClickLoggerComponent] Mouse click event received at position ({}, {})",
		event->pixelPosition.x,
		event->pixelPosition.y
	), LogLevel::INFO);
}

void MouseClickLoggerComponent::OnAdd()
{
	inputService.mouseClickBroadcast.subscribe(this);
}

void MouseClickLoggerComponent::OnRemove()
{
	inputService.mouseClickBroadcast.unsubscribe(this);
}

std::string MouseClickLoggerComponent::ToString() const
{
	return "<MouseClickLoggerComponent>";
}

std::unique_ptr<Component> MouseClickLoggerComponent::Clone(GameObject* newOwner) const
{
	return std::make_unique<MouseClickLoggerComponent>(newOwner, logger, inputService);
}
