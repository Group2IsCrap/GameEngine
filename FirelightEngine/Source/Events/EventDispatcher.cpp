#include "EventDispatcher.h"

namespace Firelight::Events
{
	std::map<Event::DescriptorType, std::vector<EventDispatcher::CallbackFunctionType>> EventDispatcher::sm_observers;

	void EventDispatcher::Subscribe(const Event::DescriptorType& descriptor, CallbackFunctionType&& callbackFunction)
	{
		sm_observers[descriptor].push_back(callbackFunction);
	}

	void EventDispatcher::Unsubscribe(const Event::DescriptorType& descriptor, const int index)
	{
		if (index < sm_observers[descriptor].size())
		{
			sm_observers[descriptor].erase(sm_observers[descriptor].begin() + index);
		}
	}

	void EventDispatcher::UnsubscribeAll(const Event::DescriptorType& descriptor)
	{
		sm_observers[descriptor].clear();
	}

	void EventDispatcher::InvokeEvent(const Event& event)
	{
		auto type = event.Type();

		// Return if no observers
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
