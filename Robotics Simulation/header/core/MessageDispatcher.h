#pragma once
#include "EventBroadcast.h"
#include "events/GhostPlacedEvent.h"
#include "events/GhostCreatedEvent.h"

/// <summary>
/// Class used for communicating between different gamesystems or components.
/// IMPORTANT: When adding a new type of event, its template class needs to be manually instantiated in core/EventBroaadcast.cpp. Othervise linker errors will occur.
/// </summary>
class CentralMessageDispatcher
{
public:
	EventBroadcast<GhostPlacedEvent> ghostPlacedEventBroadcast;
	EventBroadcast<GhostCreatedEvent> ghostCreatedEventBroadcast;
};

