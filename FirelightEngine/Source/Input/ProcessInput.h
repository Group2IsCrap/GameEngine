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

			std::shared_ptr<Input::MouseInput> m_MouseCaptuer;
			std::shared_ptr <Input::KeyboardInput> m_KeyboardCaptuer;
			std::shared_ptr <Input::ControllerManager> m_ControllerManager;

		
		};

	}
