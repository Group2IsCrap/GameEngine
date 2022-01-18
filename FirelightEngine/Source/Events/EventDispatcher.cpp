#include "EventDispatcher.h"

namespace Firelight::Events
{
	std::map<Event::DescriptorType, std::vector<Listener*>> EventDispatcher::sm_observers;

	//subscribe the function
	void EventDispatcher::Subscribe(const Event::DescriptorType& descriptor, Listener* listener)
	{
		sm_observers[descriptor].emplace_back(listener);
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

	void EventDispatcher::InvokeEvent(const Event& event, void* data)
	{
		// we want to pass the arguments here for keycode, yes a char of keycode?
		auto type = event.Type();

		if (sm_observers.find(type) == sm_observers.end())
		{
			return;
		}

		auto&& observers = sm_observers.at(type);

		for (auto&& observer : observers)
		{
			observer->HandleEvents(data);
		}
	}
}
