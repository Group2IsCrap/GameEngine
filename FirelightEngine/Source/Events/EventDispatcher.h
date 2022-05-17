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
		using DescriptorType = const char*;

		//Listener Events
		template<typename EventType>
		static void AddListener(Listener* listener);
		template<typename EventType>
		static void RemoveListener(const int index);
		template<typename EventType>
		static void RemoveAllListeners();
		template<typename EventType>
		static void InvokeListeners(void* data);
		static void InvokeListeners(DescriptorType descriptor, void* data);

		//Function Events
		template<typename EventType>
		static size_t SubscribeFunction(CallbackFunctionType&& callbackFunction);
		template<typename EventType>
		static void UnsubscribeFunction(const size_t index);
		template<typename EventType>
		static void UnsubscribeAllFunctions();
		template<typename EventType>
		static void InvokeFunctions();
		static void InvokeFunctions(DescriptorType descriptor);
		static size_t SubscribeFunction(DescriptorType descriptor,CallbackFunctionType&& callbackFunction);
	private:
		static std::map<Event::DescriptorType, std::vector<Listener*>> sm_listeners;
		static std::map<Event::DescriptorType, std::vector<CallbackFunctionType>> sm_observers;
		static std::map<Event::DescriptorType, std::unordered_map<size_t, size_t>> sm_eventMap;

	};
}

#include "EventDispatcher.inl"
