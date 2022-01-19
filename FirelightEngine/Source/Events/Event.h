#pragma once
namespace Firelight::Events
{
	class Event
	{
	public:
		virtual ~Event() {};

		using DescriptorType = const char*;

		virtual DescriptorType Type() const = 0;
	};
}