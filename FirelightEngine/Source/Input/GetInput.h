#pragma once

#include<memory>
#include"..\Maths\Vec2.h"

namespace Firelight::Input {


	enum class e_MouseButtionPress
	{
		RightButtionPress,
		LeftButtionPress,
		MiddleButtionPress
	};

	enum class e_ControllerButtion
	{
		A,
		Y,
		X,
		B,
		DPADUP,
		DPADDOWN,
		DPADLEFT,
		DPADRIGHT,
		SHOULDERRIGHT,
		SHOULDERLEFT,
		STICKPRESSRIGHT,
		STICKPRESSLEFT,
		START,
		BACK
	};
	enum class e_ControllerSticks
	{
		STICKLEFT,
		STICKRIGHT
	};
	enum class e_ControllerTrigger
	{
		LEFTTRIGGER,
		RIGHTTRIGGER
	};

	class MouseInput;
	class KeyboardInput;
	class ControllerManager;
	class GetInput
	{
		friend class ProcessInput;
	public:

		GetInput();
		GetInput(std::shared_ptr<Input::MouseInput> m_MouseCaptuer, std::shared_ptr <Input::KeyboardInput> m_KeyboardCaptuer, std::shared_ptr <Input::ControllerManager> m_ControllerManager);
		~GetInput();

		//get key input
		bool KeyIsPress(const unsigned char key);
		//set auto repeat keys/char
		void SetAutoRepeatKeys(bool isAutoRepaeat);
#

		//mouse inputs
		bool MouseButtionPress(e_MouseButtionPress MouseEvent);
		Maths::Vec2f GetMousePos();
		Maths::Vec2f GetMousePosRaw();

		//controller inputs
		bool IsButtionPress(e_ControllerButtion Buttion);
		Maths::Vec2f ControllerStickMovement(e_ControllerSticks Stick);
		float TriggerData(e_ControllerTrigger trigger);

		//setControllerSettings
		void SetDeadZone(double DeadZoneRight, double DeadZoneLeft);
		void SetTriggerThresHold(double SetTriggerThresHold);
		void SetVibration(float leftVal, float rightVal);
		
	private:
		void Initilize(std::shared_ptr<Input::MouseInput> MouseCaptuer, std::shared_ptr <Input::KeyboardInput> KeyboardCaptuer, std::shared_ptr <Input::ControllerManager> ControllerManager);
		
	public:
	private:
		std::shared_ptr<Input::MouseInput> m_MouseCaptuer;
		std::shared_ptr <Input::KeyboardInput> m_KeyboardCaptuer;
		std::shared_ptr <Input::ControllerManager> m_ControllerManager;
	};



	inline GetInput InputGet;
}
