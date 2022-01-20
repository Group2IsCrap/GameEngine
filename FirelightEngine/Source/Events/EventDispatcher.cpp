#include "EventDispatcher.h"

namespace Firelight::Events
{
	std::map<Event::DescriptorType, std::vector<Listener*>> EventDispatcher::sm_listeners;
	std::map<Event::DescriptorType, std::vector<EventDispatcher::CallbackFunctionType>> EventDispatcher::sm_observers;

	void EventDispatcher::AddListener(const Event::DescriptorType& descriptor, Listener* listener)
	{
		sm_listeners[descriptor].emplace_back(listener);
	}

	void EventDispatcher::RemoveListener(const Event::DescriptorType& descriptor, const int index)
	{
		if (index < sm_listeners[descriptor].size())
		{
			sm_listeners[descriptor].erase(sm_listeners[descriptor].begin() + index);
		}
	}

	void EventDispatcher::RemoveAllListeners(const Event::DescriptorType& descriptor)
	{
		sm_listeners[descriptor].clear();
	}

	void EventDispatcher::InvokeListeners(const Event& event, void* data)
	{
		auto type = event.Type();
		
		if (sm_listeners.find(type) == sm_listeners.end())
		{
			return;
		}

		auto&& listeners = sm_listeners.at(type);

		for (auto&& listener : listeners)
		{
			listener->HandleEvents(data);
		}
	}
	void EventDispatcher::SubscribeFunction(const Event::DescriptorType& descriptor, CallbackFunctionType&& callbackFunction)
	{
		sm_observers[descriptor].push_back(callbackFunction);
	}

	void EventDispatcher::UnsubscribeFunction(const Event::DescriptorType& descriptor, const int index)
	{
		if (index < sm_observers[descriptor].size())
		{
			sm_observers[descriptor].erase(sm_observers[descriptor].begin() + index);
		}
	}

	void EventDispatcher::UnsubscribeAllFunctions(const Event::DescriptorType& descriptor)
	{
		sm_observers[descriptor].clear();
	}

	void EventDispatcher::InvokeFunctions(const Event& event)
	{
		auto type = event.Type();

		if (sm_observers.find(type) == sm_observers.end())
		{
			return;
		}

		auto&& observers = sm_observers.at(type);

		for (auto&& observer : observers)
		{
			observer(event);
		}
	}
}
