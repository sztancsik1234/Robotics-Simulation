#include "input/LoggerButton.h"

void LoggerButton::onClick()
{
	logger.Log("Button clicked!", LogLevel::INFO);
}

void LoggerButton::onHovered()
{
	logger.Log("Button hovered!", LogLevel::TRACE);
}