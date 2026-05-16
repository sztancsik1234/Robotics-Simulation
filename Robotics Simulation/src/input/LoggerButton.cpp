#include "input/LoggerButton.h"

void LoggerButton::onClick()
{
	logger.Log("Button clicked!", LogLevel::INFO);
}

void LoggerButton::onHovered()
{
	logger.Log("Button hovered!", LogLevel::TRACE);
}

std::unique_ptr<Component> LoggerButton::Clone(GameObject* newOwner) const
{
	return std::make_unique<LoggerButton>(newOwner, GetInputService(), logger);
}
