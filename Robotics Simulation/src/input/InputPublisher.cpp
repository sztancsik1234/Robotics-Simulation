#include "input/InputPublisher.h"

template <Event_t E_t>
void InputPublisher<E_t>::subscribe(IObserver<E_t>* observer)
{
	observers.push_back(observer);
}

template <Event_t E_t>
void InputPublisher<E_t>::unsubscribe(IObserver<E_t>* observer)
{
	observers.remove(observer);
}

template <Event_t E_t>
void InputPublisher<E_t>::notify(E_t* e) const
{
	for (auto observer : observers)
	{
		observer->onNotify(e);
	}
}

template class InputPublisher<ClickEvent>;
template class InputPublisher<MouseMoveEvent>;