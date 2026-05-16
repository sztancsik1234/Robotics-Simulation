#pragma once
#include "core/Vector2.h"
#include "events/Event.h"

enum class KeyCode : char;

struct ClickEvent : public Event
{
	explicit ClickEvent(KeyCode button, Vector2 pixelPosition) : button(button), pixelPosition(pixelPosition) {}

	KeyCode button;
	Vector2 pixelPosition;
};