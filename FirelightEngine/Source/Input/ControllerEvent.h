#pragma once
#include"..\Maths\Vec2.h"
#include"..\Events\Event.h"
namespace Firelight::Events::Input 
{
	enum class ControllerButtons
	{
		A,
		B,
		X,
		Y,
		STICK_LEFT,
		STICK_RIGHT,
		DPAD_LEFT,
		DPAD_UP,
		DPAD_RIGHT,
		DPAD_DOWN,
		LB,
		RB,
		LT,
		RT,
		START,
		BACK,
		SHARE,
		HOME
	};

	//More Readable format 
	struct ControllerState
	{
		bool m_A = false;
		bool m_Y = false;
		bool m_X = false;
		bool m_B = false;

		bool m_RightBumber = false;
		bool m_LeftBumber = false;

		bool m_LeftStickPress = false;
		bool m_RightStickPress = false;
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

	NEW_EVENT(ControllerEvent);
	NEW_EVENT(OnButtonPressed);
	NEW_EVENT(OnButtonPressedSingle);
	NEW_EVENT(OnButtonReleased);
	NEW_EVENT(OnLeftThumbstickMoved);
	NEW_EVENT(OnRightThumbstickMoved);
}
