#pragma once

#include <Windows.h>

#include <Xinput.h>

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
		~ControllerInput();

		bool IsConnected();

		void ProcessInput();
		XINPUT_STATE GetState();

		void VibrateStop();
		void Vibrate(float leftVal, float rightVal);

		bool IsPressed(WORD buttion);
		bool isconnected = false;

	private:
		int m_User_ID = 0;

		int m_DeadzoneLeftThum = 0;
		int m_DeadzoneRightThum = 0;
		int m_TriggerThreshold = 0;

		double m_LeftTriggerPress = 0;
		double m_RightTriggerPress = 0;

		double m_NormalLeftThumX = 0;
		double m_NormalLeftThumY = 0;

		double m_NormalRightThumX = 0;
		double m_NormalRightThumY = 0;

		double m_MagnitudeRightThum = 0;
		double m_NormalizedRightThumbMagnitude = 0;

		double m_MagnitudeLeftThum = 0;
		double m_NormalizedLeftThumbMagnitude = 0;

		XINPUT_STATE m_State;
	};
}
