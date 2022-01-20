#include "WindowContainer.h"
#include "Input/ProcessInput.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Firelight
{
	WindowContainer::WindowContainer()
	{
		Input::ProcessInput::Instance()->Initialize();
		
	}

	LRESULT WindowContainer::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Uncomment when ImGui needs to take input
		if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		{
			return true;
		}
		//handle messages
		if (Input::ProcessInput::Instance()->HandleInput(uMsg, wParam, lParam)) {
			return true;
		}

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
