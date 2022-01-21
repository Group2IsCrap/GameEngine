#include "ProcessInput.h"

#include <cstdio>
#include <string>

#include "MouseInput.h"
#include "KeyboardInput.h"
#include "ControllerManager.h"
#include "GetInput.h"

#include "../Utils/ErrorManager.h"
#include "../Engine.h"

namespace Firelight::Input
{
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
		m_KeyboardCapture = std::make_shared<Input::KeyboardInput>();
		m_MouseCapture = std::make_shared <Input::MouseInput>();
		m_ControllerManager = std::make_shared <Input::ControllerManager>();

		result = Input::InputGet.Initialize(m_MouseCapture, m_KeyboardCapture, m_ControllerManager);
		ASSERT_RETURN(result, "GetInput failed to initialise", false);
		
		return true;
	}

	bool ProcessInput::HandleInput(UINT message, WPARAM wParam, LPARAM lParam)
	{
		// Get input from windows
		switch (message)
		{
		// Window drag event
		case WM_MOVE:
		{
			RECT rect = {0};
			GetWindowRect(Engine::Instance().GetWindowHandle(), &rect);

			int width = (int)(rect.right - rect.left);
			int height = (int)(rect.bottom - rect.top);
			Engine::Instance().SetWindowDimensions(Maths::Vec2i(width, height));
		}
		break;
		// Window resize drag event
		case WM_SIZING:
		{
			MINMAXINFO* mmiStruct = (MINMAXINFO*)lParam;
			
			int width = (int)mmiStruct->ptMaxSize.x;
			int height = (int)mmiStruct->ptMaxSize.y;
			Engine::Instance().SetWindowDimensions(Maths::Vec2i(width, height));
		}
		break;
		// Window resize button event
		case WM_SIZE:
		{
			int width = (int)LOWORD(lParam);
			int height = (int)HIWORD(lParam);
			Engine::Instance().SetWindowDimensions(Maths::Vec2i(width, height));
		}
		break;
		// Keyborad Input
		case WM_KEYUP:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);

			m_KeyboardCapture->OnKeyReplace(ch);

			return true;
		}
		break;
		case WM_KEYDOWN:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);

			if (m_KeyboardCapture->IsKeysAutoRepeat())
			{
				m_KeyboardCapture->OnKeyPress(ch);
			}
			else
			{
				const bool wasPressed = lParam & WAS_PRESSED;

				if (!wasPressed)
				{
					m_KeyboardCapture->OnKeyPress(ch);
				}
			}
			return true;
		}
		break;
		case WM_CHAR:
		{
			unsigned char ch = static_cast<unsigned char>(wParam);

			if (m_KeyboardCapture->IsCharAutoRepeat())
			{
				m_KeyboardCapture->OnChar(ch);
			}
			else
			{
				const bool wasPressed = lParam & WAS_PRESSED;

				if (!wasPressed)
				{
					m_KeyboardCapture->OnChar(ch);
				}

			}
			return true;
		}
		break;
		// Mouse input
		case WM_MOUSEMOVE:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCapture->OnMouseMove(x, y);
			return true;
		}
		break;
		case WM_LBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCapture->OnLeftPress(x, y);
			return true;
		}
		break;
		case WM_LBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCapture->OnLeftReleased(x, y);
			return true;
		}
		break;
		case WM_RBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCapture->OnRightPress(x, y);
			return true;
		}
		break;
		case WM_RBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCapture->OnRightReleased(x, y);
			return true;
		}
		break;
		case WM_MBUTTONDOWN:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCapture->OnMiddlePress(x, y);
			return true;
		}
		break;
		case WM_MBUTTONUP:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			m_MouseCapture->OnMiddleReleased(x, y);
			return true;
		}
		break;
		case WM_MOUSEWHEEL:
		{
			int x = LOWORD(lParam);
			int y = HIWORD(lParam);

			if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
			{
				m_MouseCapture->OnWheelUp(x, y);
			}
			else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
			{
				m_MouseCapture->OnWheelDown(x, y);
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
						m_MouseCapture->OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
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
