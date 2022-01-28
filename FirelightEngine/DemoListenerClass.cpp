#include "DemoListenerClass.h"
#include "Source/Events/EventDispatcher.h"
#include "Source/Events/DemoEvent.h"

namespace Firelight::DemoNamespace
{
	DemoListenerClass::DemoListenerClass()
	{
		Firelight::Events::EventDispatcher::AddListener<Firelight::Events::DemoNamespace::DemoEvent>(this);
	}

	void DemoListenerClass::HandleEvents(const char* event, void* data)
	{
		int number = int(data);
		// do stuff with data.
	}
}
