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
	
		const Window& GetWindow() const;
		Window&       GetWritableWindow();

	protected:
		Window m_window;
	};
}
