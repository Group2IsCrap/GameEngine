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

		//using CallbackFunctionType = std::function< void(const Event&) >;

		static void Subscribe(const Event::DescriptorType& descriptor, Listener* listener);//, CallbackFunctionType&& callbackFunction);
		//Add a listener class

		static void Unsubscribe(const Event::DescriptorType& descriptor, const int index);

		static void UnsubscribeAll(const Event::DescriptorType& descriptor);

		static void InvokeEvent(const Event& event, void* data);
		//This will be called in the input I think. We subcribe in the player class


	private:

		static std::map<Event::DescriptorType, std::vector<Listener*>> sm_observers;
	};
}
