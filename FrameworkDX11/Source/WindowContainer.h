#pragma once

#include "Window.h"

// Layer where window input is processed

namespace TempEngineNamespace
{
	class WindowContainer
	{
	public:
		WindowContainer();

		LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		Window& GetWindow();

	protected:
		Window mWindow;
	};
}
