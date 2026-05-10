#pragma once
#include "util/Event.h"
#include <type_traits>

template<typename T>
concept Event_t = std::is_base_of_v<Event, T>;

template<Event_t e_t>
struct IObserver
{
public:
	virtual ~IObserver() = default;
	virtual void onNotify(e_t* e) = 0;
};