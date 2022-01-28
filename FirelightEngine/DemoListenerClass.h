#pragma once
#include "Source/Events/Listener.h"

namespace Firelight::DemoNamespace
{
	class DemoListenerClass : public Firelight::Events::Listener
	{
		DemoListenerClass();

		virtual void HandleEvents(const char* event, void* data) override;
	};
}

