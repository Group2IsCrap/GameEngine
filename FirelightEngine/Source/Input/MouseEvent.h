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
		Maths::Vec2f GetPos();

		int GetMouseX();
		int GetMouseY();

		static constexpr DescriptorType sm_Description = "MouseEvent";

		virtual DescriptorType Type() const override
		{
			return sm_Description;
		}

	public:
		e_MouseEventType m_Type;
		int m_XPos;
		int m_YPos;
	};

	class MouseButtionReleaseEvent :public MouseEvent
	{
	public:



		MouseButtionReleaseEvent();
		MouseButtionReleaseEvent(const e_MouseEventType type, const int x, const int y);
		~MouseButtionReleaseEvent();

		static constexpr DescriptorType sm_Description = "Mouse_Buttion_Release";

		virtual DescriptorType Type() const override
		{
			return sm_Description;
		}


	};
	class MouseButtionPressEvent :public MouseEvent
	{
	public:



		MouseButtionPressEvent();
		MouseButtionPressEvent(const e_MouseEventType type, const int x, const int y);
		~MouseButtionPressEvent();

		static constexpr DescriptorType sm_Description = "Mouse_Buttion_Press";

		virtual DescriptorType Type() const override
		{
			return sm_Description;
		}


	};



	class MouseMoveEvent :public MouseEvent
	{
	public:



		MouseMoveEvent();
		MouseMoveEvent(const e_MouseEventType type, const int x, const int y);
		~MouseMoveEvent();

		static constexpr DescriptorType sm_Description = "Mouse_Move";

		virtual DescriptorType Type() const override
		{
			return sm_Description;
		}


	};

	class MouseMoveRawEvent :public MouseEvent
	{
	public:



		MouseMoveRawEvent();
		MouseMoveRawEvent(const e_MouseEventType type, const int x, const int y);
		~MouseMoveRawEvent();

		static constexpr DescriptorType sm_Description = "Mouse_Move_Raw";

		virtual DescriptorType Type() const override
		{
			return sm_Description;
		}


	};

}