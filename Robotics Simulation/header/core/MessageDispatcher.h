#pragma once
#include "EventBroadcast.h"
#include "events/GhostPlacedEvent.h"

/// <summary>
/// Class used for communicating between different gamesystems or components.
/// </summary>
class CentralMessageDispatcher
{
	EventBroadcast<GhostPlacedEvent> ghostPlacedEventBroadcast;
};

