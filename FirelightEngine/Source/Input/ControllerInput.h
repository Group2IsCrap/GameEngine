#pragma once
#include<Windows.h>
#include<Xinput.h>
#include"..\Maths\Vec2.h"
#include"..\Events\Event.h"
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



	struct ControllerState
	{
		bool A= false;
		bool Y = false;
		bool X = false;
		bool B = false;

		bool RightBumber = false;
		bool LeftBumber = false;

		bool StickLeftPress = false;
		bool StickRightPress = false;
		bool Back = false;
		bool Start = false;

		bool DPADUp = false;
		bool DPADDown = false;
		bool DPADLeft = false;
		bool DPADRight = false;
		Maths::Vec2f LeftStick = Maths::Vec2f();
		Maths::Vec2f  RightStick= Maths::Vec2f();
		int LeftTrigger=0;
		int RightTrigger=0;

	};


}
namespace Firelight::Events::Input {
	class ContollerEvent : public Events::Event
	{
	public:
		ContollerEvent();
		~ContollerEvent();

		static constexpr DescriptorType sm_Des = "ControllerState";

		virtual DescriptorType Type() const override
		{
			return sm_Des;
		}


	};
}