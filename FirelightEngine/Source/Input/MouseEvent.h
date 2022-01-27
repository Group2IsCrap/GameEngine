#pragma once

#include"..\Events\Event.h"

#include"..\Maths\Vec2.h"

namespace Firelight::Events::Input 
{
	

	enum class e_MouseEventType
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			WheelUp,
			WheelDown,
			Move,
			RawMove,
			Invalid
		};

	class MouseEvent:public Event
	{
	public:
		MouseEvent();
		MouseEvent(const e_MouseEventType type, const int x, const int y);
		~MouseEvent();

		bool IsValid();
		e_MouseEventType GetType();
		const Maths::Vec2i& GetPos();

		int GetMouseX();
		int GetMouseY();

		static constexpr DescriptorType sm_descriptor = "MouseEvent";

	public:
		e_MouseEventType m_Type;
		Maths::Vec2i     m_Pos;
	};

	class MouseButtonReleaseEvent :public MouseEvent
	{
	public:
		MouseButtonReleaseEvent();
		~MouseButtonReleaseEvent();
		MouseButtonReleaseEvent(const e_MouseEventType type, const int x, const int y);
		

		static constexpr DescriptorType sm_descriptor = "Mouse_Buttion_Release";
	};

	class MouseButtonPressEvent :public MouseEvent
	{
	public:
		MouseButtonPressEvent();
		MouseButtonPressEvent(const e_MouseEventType type, const int x, const int y);
		~MouseButtonPressEvent();

		static constexpr DescriptorType sm_descriptor = "Mouse_Buttion_Press";
	};

	class MouseMoveEvent :public MouseEvent
	{
	public:
		MouseMoveEvent();
		MouseMoveEvent(const e_MouseEventType type, const int x, const int y);
		~MouseMoveEvent();

		static constexpr DescriptorType sm_descriptor = "Mouse_Move";
	};

	class MouseMoveRawEvent :public MouseEvent
	{
	public:
		MouseMoveRawEvent();
		MouseMoveRawEvent(const e_MouseEventType type, const int x, const int y);
		~MouseMoveRawEvent();

		static constexpr DescriptorType sm_descriptor = "Mouse_Move_Raw";
	};
}
