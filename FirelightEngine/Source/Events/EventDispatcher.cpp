#include "EventDispatcher.h"

namespace Firelight::Events
{
	std::map<Event::DescriptorType, std::vector<Listener*>> EventDispatcher::sm_listeners;
	std::map<Event::DescriptorType, std::vector<EventDispatcher::CallbackFunctionType>> EventDispatcher::sm_observers;
}
