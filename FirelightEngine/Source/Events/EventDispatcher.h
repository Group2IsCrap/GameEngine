#pragma once
#include "Event.h"
#include "Listener.h"

#include <functional>
#include <map>
#include <vector>

namespace Firelight::Events
{
	class EventDispatcher
	{
	public:
		using CallbackFunctionType = std::function< void() >;

		//Listener Events
		static void AddListener(const Event::DescriptorType& descriptor, Listener* listener);
		static void RemoveListener(const Event::DescriptorType& descriptor, const int index);
		static void RemoveAllListeners(const Event::DescriptorType& descriptor);
		static void InvokeListeners(const Event& event, void* data);

		//Function Events
		static void SubscribeFunction(const Event::DescriptorType& descriptor, CallbackFunctionType&& callbackFunction);
		static void UnsubscribeFunction(const Event::DescriptorType& descriptor, const int index);
		static void UnsubscribeAllFunctions(const Event::DescriptorType& descriptor);
		template<typename EventType>
		static void InvokeFunctions()
		{
			auto type = EventType::Type();

			if (sm_observers.find(type) == sm_observers.end())
			{
				return;
			}

			auto&& observers = sm_observers.at(type);

			for (auto&& observer : observers)
			{
				observer();
			}
		}


	private:
		static std::map<Event::DescriptorType, std::vector<Listener*>> sm_listeners;
		static std::map<Event::DescriptorType, std::vector<CallbackFunctionType>> sm_observers;
	};
}
