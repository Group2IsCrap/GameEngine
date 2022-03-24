#pragma once

#include<memory>
#include"..\Maths\Vec2.h"

namespace Firelight::Input {


	enum class e_MouseButtonPress
	{
		RIGHTBUTTONPRESS,
		LEFTBUTTONPRESS,
		MIDDLEBUTTONPRESS
	};

	enum class e_ControllerButton
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
		GetInput(std::shared_ptr<Input::MouseInput> mouseCapture, std::shared_ptr <Input::KeyboardInput> keyboardCapture, std::shared_ptr <Input::ControllerManager> controllerManager);
		~GetInput();

		//get key input
		bool KeyIsPress(const unsigned char key);
		bool KeyIsPressNonRepeat(const unsigned char key);

		//set auto repeat keys/char
		//void SetAutoRepeatKeys(bool isAutoRepeat);
#

		//mouse inputs
		bool MouseButtonPress(e_MouseButtonPress mouseButton);
		Maths::Vec2i GetMousePos();
		Maths::Vec2i GetMousePosRaw();

		//controller inputs
		bool IsButtonPress(e_ControllerButton button);
		Maths::Vec2f ControllerStickMovement(e_ControllerSticks stick);
		float TriggerData(e_ControllerTrigger trigger);

		//setControllerSettings
		void SetDeadZone(double deadzoneRight, double deadzoneLeft);
		void SetTriggerThresHold(double setTriggerThreshold);
		void SetVibration(float leftVal, float rightVal);
		
	private:
		bool Initialize(std::shared_ptr<Input::MouseInput> mouseCapture, std::shared_ptr <Input::KeyboardInput> KeyboardCapture, std::shared_ptr <Input::ControllerManager> controllerManager);
		
	public:
	private:
		std::shared_ptr<Input::MouseInput> m_MouseCapture;
		std::shared_ptr <Input::KeyboardInput> m_KeyboardCapture;
		std::shared_ptr <Input::ControllerManager> m_ControllerManager;
	};



	inline GetInput InputGet;
}
