#pragma once
#include "Listener.h"
#include <iostream>

namespace Firelight::Events
{

	class EventListener : public Listener
	{
		void functionCallback(int num)
		{
			std::cout << num;
		}


		virtual void HandleEvents(void* data) override
		{
			//functionCallback((int)data);
			//This will decide what to do with the event.
		}
	};
}