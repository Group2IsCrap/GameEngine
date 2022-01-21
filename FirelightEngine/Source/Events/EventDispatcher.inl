#pragma once

#include "EventDispatcher.h"

namespace Firelight::Events
{
	template<typename EventType>
	void EventDispatcher::AddListener(Listener* listener)
	{
		sm_listeners[EventType::sm_descriptor].emplace_back(listener);
	}

	template<typename EventType>
	void EventDispatcher::RemoveListener(const int index)
	{
		if (index < sm_listeners[EventType::sm_descriptor].size())
		{
			sm_listeners[EventType::sm_descriptor].erase(sm_listeners[EventType::sm_descriptor].begin() + index);
		}
	}

	template<typename EventType>
	void EventDispatcher::RemoveAllListeners()
	{
		sm_listeners[EventType::sm_descriptor].clear();
	}

	template<typename EventType>
	void EventDispatcher::InvokeListeners(void* data)
	{
		if (sm_listeners.find(EventType::sm_descriptor) == sm_listeners.end())
		{
			return;
		}

		auto&& listeners = sm_listeners.at(EventType::sm_descriptor);

		for (auto&& listener : listeners)
		{
			listener->HandleEvents(data);
		}
	}

	template<typename EventType>
	void EventDispatcher::SubscribeFunction(CallbackFunctionType&& callbackFunction)
	{
		sm_observers[EventType::sm_descriptor].push_back(callbackFunction);
	}

	template<typename EventType>
	void EventDispatcher::UnsubscribeFunction(const int index)
	{
		if (index < sm_observers[EventType::sm_descriptor].size())
		{
			sm_observers[EventType::sm_descriptor].erase(sm_observers[EventType::sm_descriptor].begin() + index);
		}
	}

	template<typename EventType>
	void EventDispatcher::UnsubscribeAllFunctions()
	{
		sm_observers[EventType::sm_descriptor].clear();
	}

	template<typename EventType>
	inline void EventDispatcher::InvokeFunctions()
	{
		if (sm_observers.find(EventType::sm_descriptor) == sm_observers.end())
		{
			return;
		}

		auto&& observers = sm_observers.at(EventType::sm_descriptor);

		for (auto&& observer : observers)
		{
			observer();
		}
	}
}