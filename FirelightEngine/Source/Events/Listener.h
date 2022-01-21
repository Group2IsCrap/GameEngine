#pragma once

namespace Firelight::Events
{
	class Listener
	{
	public:
		virtual void HandleEvents(void* data) = 0;
	};
}