#pragma once
#include <list>
#include "util/IObserver.h"
#include "events/ClickEvent.h"
#include "events/MouseMoveEvent.h"

// deprecated
template <Event_t E_t>
class InputPublisher
{
public:
	InputPublisher() = default;

	void subscribe(IObserver<E_t>* observer);
	void unsubscribe(IObserver<E_t>* observer);


private:
	friend class SfmlInputService;

	/// <summary>
	/// notify the observers about an event.
	/// </summary>
	void notify(E_t* e) const;
	std::list<IObserver<E_t>*> observers;
};

