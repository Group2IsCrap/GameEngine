#pragma once
#include<Windows.h>
#include<memory>
#define WAS_PRESSED 0x40000000;

	class MouseInput;
	class KeyboardInput;
	class ControllerManager;
	class ProcessInput
	{
	public:
		static ProcessInput* Instance();

		void Initialize();
		bool HandleInput(UINT message, WPARAM wParam, LPARAM lParam);
		void ControllerInput();
		void TestInput();
	private:
		ProcessInput();
		~ProcessInput();
	private:

		std::unique_ptr<MouseInput> m_MouseCaptuer;
		std::unique_ptr <KeyboardInput> m_KeyboardCaptuer;
		std::unique_ptr <ControllerManager> m_ControllerManager;
	};


