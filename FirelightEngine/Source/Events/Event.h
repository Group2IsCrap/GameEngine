#pragma once
namespace Firelight::Events
{
#define BIT(x) (1 << x)

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4),
		EventCategoryEngine = BIT(5),
		EventCategoryGame = BIT(6)
	};


	class Event
	{
	public:
		virtual ~Event() {};

		using DescriptorType = const char*;

		virtual DescriptorType Type() const = 0;
	};
}