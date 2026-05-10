#include "core/EventBroadcast.h"
#include "events/GhostPlacedEvent.h"

template <Event_t E_t>
void EventBroadcast<E_t>::subscribe(IObserver<E_t>* observer)
{
	subscribers.push_back(observer);
}

template <Event_t E_t>
void EventBroadcast<E_t>::unsubscribe(IObserver<E_t>* observer)
{
	subscribers.remove(observer);
}

template <Event_t E_t>
void EventBroadcast<E_t>::broadcast(E_t* e) const
{
	for (auto subscriber : subscribers)
	{
		subscriber->onNotify(e);
	}
}

template class EventBroadcast<GhostPlacedEvent>;