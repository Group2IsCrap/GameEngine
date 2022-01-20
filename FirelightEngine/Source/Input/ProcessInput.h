#pragma once

#include <Windows.h>
#include <memory>

#define WAS_PRESSED 0x40000000;

	namespace Firelight::Input 
	{
		#define WAS_PRESSED 0x40000000;

		class MouseInput;
		class KeyboardInput;
		class ControllerManager;

		/// <summary>
		/// takes inputs from windows and process it to be used for events
		/// tells the controller manger chaeck state of controllers evey frame
		/// </summary>
		class ProcessInput
		{
		public:
			static ProcessInput* Instance();

			bool Initialize();
			bool HandleInput(UINT message, WPARAM wParam, LPARAM lParam);
			void ControllerInput();
			void TestInput();
		private:
			ProcessInput();
			~ProcessInput();
		private:

			std::shared_ptr<Input::MouseInput> m_MouseCapture;
			std::shared_ptr <Input::KeyboardInput> m_KeyboardCapture;
			std::shared_ptr <Input::ControllerManager> m_ControllerManager;
		};

		std::unique_ptr<Input::MouseInput> m_MouseCapture;
		std::unique_ptr<Input::KeyboardInput> m_KeyboardCapture;
		std::unique_ptr<Input::ControllerManager> m_ControllerManager;
	};
}
