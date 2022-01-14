#pragma once
#include<Windows.h>
#include<memory>
#define WAS_PRESSED 0x40000000;



	namespace Firelight::Input {
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

			std::unique_ptr<Input::MouseInput> m_MouseCaptuer;
			std::unique_ptr <Input::KeyboardInput> m_KeyboardCaptuer;
			std::unique_ptr <Input::ControllerManager> m_ControllerManager;
		};

	}
