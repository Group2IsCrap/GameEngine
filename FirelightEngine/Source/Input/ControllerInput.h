#pragma once
#include<Windows.h>
#include<Xinput.h>
#include"..\Maths\Vec2.h"

#pragma comment(lib, "Xinput.lib")

namespace Firelight::Input {
	/// <summary>
	/// XBox 360 Controller Support
	/// </summary>
	class ControllerInput
	{
	public:
		ControllerInput();
		ControllerInput(int ControllerIndex, double DeadZoneLeftThumb, double DeadZoneRightThumb, double TriggerThreshold);
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

		void SetDeadZoneLeftThumb(double Deadzone);
		void SetDeadZoneRightThumb(double Deadzone);
		void SetTriggerThreshold(double TriggerThreshold);
	private:
		bool IsConnected();
	public:
		bool m_Isconnected = false;


	private:
		int m_User_ID = 0;

		double m_DeadzoneLeftThum = 0;
		double m_DeadzoneRightThum = 0;
		double m_TriggerThreshold = 0;

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
