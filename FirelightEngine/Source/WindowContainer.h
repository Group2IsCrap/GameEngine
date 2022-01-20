#pragma once

#include "Window.h"

// Layer where window input is processed

namespace Firelight
{
	class WindowContainer
	{
	public:
		WindowContainer();

		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		Window& GetWindow();

	protected:
		Window m_window;
	};
}
