#include "ProcessInput.h"
#include<cstdio>
#include<string>
#include"MouseInput.h"
#include"KeyboardInput.h"
#include"ControllerManager.h"
#include"GetInput.h"
#include"..\Utils\ErrorManager.h"
namespace Firelight::Input {
	ProcessInput::ProcessInput()
	{
	}

	ProcessInput* ProcessInput::Instance()
	{
		static ProcessInput Instance;
		return &Instance;
	}

	ProcessInput::~ProcessInput()
	{
	}
	bool ProcessInput::Initialize()
	{
		bool result;
		m_KeyboardCaptuer = std::make_shared<Input::KeyboardInput>();
		m_MouseCaptuer = std::make_shared <Input::MouseInput>();
		m_ControllerManager = std::make_shared <Input::ControllerManager>();

		result = Input::InputGet.Initialize(m_MouseCaptuer, m_KeyboardCaptuer, m_ControllerManager);
		ASSERT_RETURN(result, "GetInput failed to initialise", false);
		
		return true;
	}

	bool ProcessInput::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
	{

		//get input from windows
		switch (message)
		{
			//Keyborad Input
		case WM_KEYUP: {
			unsigned char ch = static_cast<unsigned char>(wParam);
			m_KeyboardCaptuer->OnKeyRelace(ch);
			return true;
		}
					 break;
		case WM_KEYDOWN: {
			unsigned char ch = static_cast<unsigned char>(wParam);
			if (m_KeyboardCaptuer->IsKeysAutoRepat()) {

				m_KeyboardCaptuer->OnKeyPress(ch);
			}
			else
			{
				const bool wasPressed = lParam & WAS_PRESSED;
				if (!wasPressed) {
					m_KeyboardCaptuer->OnKeyPress(ch);
				}

			}
			return true;
		}
					   break;
		case WM_CHAR:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);
			if (m_KeyboardCaptuer->IsCharAutoRepat()) {
				m_KeyboardCaptuer->OnChar(ch);
			}
			else
			{
				const bool wasPressed = lParam & WAS_PRESSED;
				if (!wasPressed) {
					m_KeyboardCaptuer->OnChar(ch);
				}

			}
			return true;
		}
		break;
		//mouse input
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCaptuer->OnMouseMove(x, y);
			return true;
		}
		break;
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCaptuer->OnLeftPress(x, y);
			return true;
		}
		break;
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCaptuer->OnLeftReleased(x, y);
			return true;
		}
		break;
		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCaptuer->OnRightPress(x, y);
			return true;
		}
		break;
		case WM_RBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCaptuer->OnRightReleased(x, y);
			return true;
		}
		break;
		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCaptuer->OnMiddlePress(x, y);
			return true;
		}
		break;
		case WM_MBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCaptuer->OnMiddleReleased(x, y);
			return true;
		}
		break;
		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);
			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
				m_MouseCaptuer->OnWheelUp(x, y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
				m_MouseCaptuer->OnWheelDown(x, y);
			}



			return true;
		}
		break;

		case WM_INPUT:
		{
			UINT dataSize = 0u;
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));
			if (dataSize > 0)
			{
				std::unique_ptr<BYTE[]> rawData = std::make_unique<BYTE[]>(dataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawData.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawData.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						m_MouseCaptuer->OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
					}
				}
			}

			return false;

		}
		break;
		}


		return false;
	}

	void ProcessInput::ControllerInput()
	{
		m_ControllerManager->HandleInput();
	}

	void ProcessInput::TestInput()
	{
		
	}
}