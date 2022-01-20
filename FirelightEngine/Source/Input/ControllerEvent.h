#pragma once
#include"..\Maths\Vec2.h"
#include"..\Events\Event.h"
namespace Firelight::Events::Input 
{

	//More Readable format 
	struct ControllerState
	{
		bool m_A = false;
		bool m_Y = false;
		bool m_X = false;
		bool m_B = false;

		bool m_RightBumber = false;
		bool m_LeftBumber = false;

		bool m_StickLeftPress = false;
		bool m_StickRightPress = false;
		bool m_Back = false;
		bool m_Start = false;

		bool m_DPadUp = false;
		bool m_DPadDown = false;
		bool m_DPadLeft = false;
		bool m_DPadRight = false;
		Maths::Vec2f m_LeftStick = Maths::Vec2f();
		Maths::Vec2f  m_RightStick = Maths::Vec2f();
		double m_LeftTrigger = 0;
		double m_RightTrigger = 0;

	};

	class ContollerEvent : public Events::Event
	{
	public:
		ContollerEvent() {}
		~ContollerEvent() {}

		static constexpr DescriptorType sm_Description = "ControllerState";

		virtual DescriptorType Type() const override
		{
			return sm_Description;
		}


	};
}