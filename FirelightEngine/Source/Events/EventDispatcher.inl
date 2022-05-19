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
		InvokeListeners(EventType::sm_descriptor, data);
	}

	inline void EventDispatcher::InvokeListeners(DescriptorType descriptor, void* data)
	{
		if (!sm_listeners.contains(descriptor))
		{
			return;
		}

		auto&& listeners = sm_listeners.at(descriptor);

		for (auto&& listener : listeners)
		{
			listener->HandleEvents(descriptor, data);
		}
	}

	template<typename EventType>
	size_t EventDispatcher::SubscribeFunction(CallbackFunctionType&& callbackFunction)
	{
		sm_observers[EventType::sm_descriptor].push_back(callbackFunction);
		size_t index = sm_observers[EventType::sm_descriptor].size() - 1;

		sm_eventMap[EventType::sm_descriptor].insert({ index,index });

		return index;
	}

	template<typename EventType>
	inline void EventDispatcher::UnsubscribeFunction(const size_t index)
	{
		size_t trueIndex = sm_eventMap[EventType::sm_descriptor][index];
		if (trueIndex < sm_observers[EventType::sm_descriptor].size())
		{
			sm_observers[EventType::sm_descriptor].erase(sm_observers[EventType::sm_descriptor].begin() + trueIndex);
			sm_eventMap[EventType::sm_descriptor].erase(index);

			for (auto& ele : sm_eventMap[EventType::sm_descriptor])
			{
				if (ele.second > trueIndex)
				{
					ele.second--;
				}
			}
		}
	}


	template<typename EventType>
	inline void EventDispatcher::UnsubscribeAllFunctions()
	{
		sm_observers[EventType::sm_descriptor].clear();
	}

	template<typename EventType>
	inline void EventDispatcher::InvokeFunctions()
	{
		InvokeFunctions(EventType::sm_descriptor);
	}

	template<typename EventType>
	inline void EventDispatcher::InvokeFunctions(void* data)
	{
		InvokeFunctions(EventType::sm_descriptor, data);
	}

	inline void EventDispatcher::InvokeFunctions(DescriptorType descriptor)
	{
		if (!sm_observers.contains(descriptor))
		{
			return;
		}

		auto&& observers = sm_observers.at(descriptor);

		for (auto&& observer : observers)
		{
			observer(nullptr);
		}
	}

	inline void EventDispatcher::InvokeFunctions(DescriptorType descriptor, void* data)
	{
		if (!sm_observers.contains(descriptor))
		{
			return;
		}

		auto&& observers = sm_observers.at(descriptor);

		for (auto&& observer : observers)
		{
			observer(data);
		}
	}

	inline size_t EventDispatcher::SubscribeFunction(DescriptorType descriptor, CallbackFunctionType&& callbackFunction)
	{
		sm_observers[descriptor].push_back(callbackFunction);
		size_t index = sm_observers[descriptor].size() - 1;

		sm_eventMap[descriptor].insert({ index,index });

		return index;
	}
}
