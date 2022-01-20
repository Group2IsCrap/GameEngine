#pragma once

#include <Windows.h>
#include <Xinput.h>

#include"..\Maths\Vec2.h"

#pragma comment(lib, "Xinput.lib")

namespace Firelight::Input
{
	/// <summary>
	/// XBox 360 Controller Support
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
		bool IsPressed(WORD buttion);

		Maths::Vec2f GetLeftStickState();
		Maths::Vec2f GetRightStickState();
		float GetRightTriggerState();
		float GetLeftTriggerState();

		void SetDeadZoneLeftThumb(double deadzone);
		void SetDeadZoneRightThumb(double deadzone);
		void SetTriggerThreshold(double triggerThreshold);

	private:
		bool IsConnected();

	public:
		bool m_Isconnected = false;

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
	};
}
