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
		// Handle messages for game
		if (Input::ProcessInput::Instance()->HandleInput(uMsg, wParam, lParam))
		{
			return true;
		}

		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	
	const Window& WindowContainer::GetWindow() const
	{
		return m_window;
	}

	Window& WindowContainer::GetWritableWindow()
	{
		return m_window;
	}

}
