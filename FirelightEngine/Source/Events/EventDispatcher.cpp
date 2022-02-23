#include "EventDispatcher.h"

namespace Firelight::Events
{
	std::map<Event::DescriptorType, std::vector<Listener*>> EventDispatcher::sm_listeners;
	std::map<Event::DescriptorType, std::vector<EventDispatcher::CallbackFunctionType>> EventDispatcher::sm_observers;
	std::map<Event::DescriptorType, std::unordered_map<size_t, size_t>> EventDispatcher::sm_eventMap;
}
