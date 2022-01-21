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

#define NEW_EVENT(eventName) \
	class eventName : public Event\
	{\
	public:\
		static constexpr DescriptorType sm_descriptor = "eventName";\
		virtual DescriptorType Type() const override { return sm_descriptor; }\
	};
}