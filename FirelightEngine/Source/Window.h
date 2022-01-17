#pragma once

#include <windows.h>
#include <windowsx.h>

#include "Utils/ErrorManager.h"

// Forward declare
namespace Firelight
{
	class WindowContainer;
}

namespace Firelight
{
	class Window
	{
	public:
		Window();
		~Window();

		bool Initialise(WindowContainer* windowContainer, HINSTANCE hInstance, const char* windowTitle, std::string windowClass, int width, int height);
		bool ProcessMessages();
		HWND GetHWND() const;
		void Destroy();

	private:
		void RegisterWindowClass();
	private:
		HWND         m_handle = NULL;
		HINSTANCE    m_hInstance = NULL;

		const char*  m_windowTitle = "";
		std::wstring m_windowTitleWide = L"";
		std::string  m_windowClass = "";
		std::wstring m_windowClassWide = L"";

		int          m_width = 0;
		int          m_height = 0;
		bool m_isDestroyed = false;
	};
}
