#pragma once
#include "core/Vector2.h"
#include "events/Event.h"

struct MouseMoveEvent : public Event
{
	explicit MouseMoveEvent(Vector2 position) : Position(position) {}

	Vector2 Position;
};