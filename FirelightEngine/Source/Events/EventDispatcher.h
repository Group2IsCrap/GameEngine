#pragma once
#include "Event.h"

#include <functional>
#include <map>
#include <vector>

namespace Firelight::Events
{
	class EventDispatcher
	{
	public:

		using CallbackFunctionType = std::function< void(const Event&) >;

		static void Subscribe(const Event::DescriptorType& descriptor, CallbackFunctionType&& callbackFunction);

		static void Unsubscribe(const Event::DescriptorType& descriptor, const int index);

		static void UnsubscribeAll(const Event::DescriptorType& descriptor);

		static void InvokeEvent(const Event& event);

	private:

		static std::map<Event::DescriptorType, std::vector<CallbackFunctionType>> sm_observers;
	};
}
