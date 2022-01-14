#include "WindowContainer.h"

//extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Firelight
{
	WindowContainer::WindowContainer()
	{
		static bool rawInputInitialized = false;
		if (rawInputInitialized == false)
		{
			RAWINPUTDEVICE rawInputDevice;

			rawInputDevice.usUsagePage = 0x01;
			rawInputDevice.usUsage = 0x02;
			rawInputDevice.dwFlags = 0;
			rawInputDevice.hwndTarget = 0;

			if (RegisterRawInputDevices(&rawInputDevice, 1, sizeof(rawInputDevice)) == FALSE)
			{
				COM_ERROR_FATAL(GetLastError(), "Failed to register raw input devices.");
			}

			rawInputInitialized = true;
		}
	}

	LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Uncomment when ImGui needs to take input
		/*if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}*/

		switch (uMsg)
		{
		// Keyboard messages
		case WM_CHAR:
		{
			// TODO: Handle char messages
		}
		case WM_KEYDOWN:
		{
			// TODO: Handle keydown messages
		}
		case WM_KEYUP:
		{
			// TODO: Handle keyup messages
		}
		// Mouse messages
		case WM_MOUSEMOVE:
		{
			// TODO: Handle mouse move messages
		}
		case WM_INPUT:
		{
			UINT dataSize = 0;

			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

			if (dataSize > 0)
			{
				std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
				if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
				{
					RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
					if (raw->header.dwType == RIM_TYPEMOUSE)
					{
						// TODO: Handle raw mouse move messages
					}
				}
			}

			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		case WM_LBUTTONDOWN:
		{
			// TODO: Handle left mouse down messages
		}
		case WM_RBUTTONDOWN:
		{
			// TODO: Handle right mouse down messages
		}
		case WM_MBUTTONDOWN:
		{
			// TODO: Handle middle mouse down messages
		}
		case WM_LBUTTONUP:
		{
			// TODO: Handle left mouse up messages
		}
		case WM_RBUTTONUP:
		{
			// TODO: Handle right mouse up messages
		}
		case WM_MBUTTONUP:
		{
			// TODO: Handle middle mouse up messages
		}
		case WM_MOUSEWHEEL:
		{
			// TODO: Handle mouse wheel messages
		}
		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	Window& WindowContainer::GetWindow()
	{
		return m_window;
	}
}
