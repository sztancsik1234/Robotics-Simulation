#pragma once

#include <list>
#include "util/IObserver.h"

template <Event_t E_t>
class EventBroadcast
{
public:
	EventBroadcast() = default;

	void subscribe(IObserver<E_t>* observer);
	void unsubscribe(IObserver<E_t>* observer);

	/// <summary>
	/// notify the observers about an event.
	/// </summary>
	void broadcast(E_t* e) const;

private:

	std::list<IObserver<E_t>*> subscribers;
};

