#pragma once

namespace Firelight::Events
{

	class Listener
	{
		//Sender
	public:

		virtual void HandleEvents(void* data);
	private:

	};
}