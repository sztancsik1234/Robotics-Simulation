#pragma once
#include "events/Event.h"
#include "core/GameObject.h"

class GhostCreatedEvent final : public Event
{
public:
	explicit GhostCreatedEvent(GameObject* createdGhost) : 
		createdGhost(createdGhost)
	{}
	GameObject* createdGhost;
};
