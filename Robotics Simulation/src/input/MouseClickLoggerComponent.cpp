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
	inputService.mouseClickSubject.subscribe(this);
}

void MouseClickLoggerComponent::OnRemove()
{
	inputService.mouseClickSubject.unsubscribe(this);
}

std::string MouseClickLoggerComponent::ToString() const
{
	return "<MouseClickLoggerComponent>";
}
