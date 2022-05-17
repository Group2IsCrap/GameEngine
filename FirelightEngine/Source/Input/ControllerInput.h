#pragma once

#include <Windows.h>
#include <Xinput.h>

#include"..\Maths\Vec2.h"

#include "ControllerEvent.h"

#pragma comment(lib, "Xinput.lib")

using namespace Firelight::Events::Input;

namespace Firelight::Input
{
	/// <summary>
	/// XBox Controller Support
	/// </summary>
	class ControllerInput
	{
	public:
		ControllerInput();
		ControllerInput(int controllerIndex, double deadZoneLeftThumb, double deadZoneRightThumb, double triggerThreshold);
		~ControllerInput();

		void ProcessInput();
		XINPUT_STATE GetState();

		void VibrateStop();
		void Vibrate(float leftVal, float rightVal);

		void TestInput();
		bool IsPressed(WORD button);

		Maths::Vec2f GetLeftStickState();
		Maths::Vec2f GetRightStickState();
		float GetRightTriggerState();
		float GetLeftTriggerState();

		void SetDeadZoneLeftThumb(double deadzone);
		void SetDeadZoneRightThumb(double deadzone);
		void SetTriggerThreshold(double triggerThreshold);
		bool m_InputThis = false;
	private:
		bool IsConnected();
		void ProcessStateChange(Events::Input::ControllerState state);

		void ProcessButtonPressed(bool state, bool previousState, ControllerButtons button);

	public:
		bool m_IsConnected = false;

	private:
		int m_User_ID = 0;

		double m_DeadzoneLeftThumb = 0;
		double m_DeadzoneRightThumb = 0;
		double m_TriggerThreshold = 0;

		double m_LeftTriggerPress = 0;
		double m_RightTriggerPress = 0;

		double m_NormalLeftThumbX = 0;
		double m_NormalLeftThumbY = 0;

		double m_NormalRightThumbX = 0;
		double m_NormalRightThumbY = 0;

		double m_MagnitudeRightThumb = 0;
		double m_NormalizedRightThumbMagnitude = 0;

		double m_MagnitudeLeftThumb = 0;
		double m_NormalizedLeftThumbMagnitude = 0;

		XINPUT_STATE m_State;
		POINT m_CurrentMousePos;
		
		Events::Input::ControllerState m_previousState;
	};
}
