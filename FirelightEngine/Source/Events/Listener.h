#pragma once

namespace Firelight::Events
{
	class Listener
	{
	public:
		virtual void HandleEvents(const char* event, void* data) = 0;
	};
}