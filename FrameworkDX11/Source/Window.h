#pragma once

#include <windows.h>
#include <windowsx.h>

#include "Utils/ErrorManager.h"

// Forward declare
namespace TempEngineNamespace
{
	class WindowContainer;
}

namespace TempEngineNamespace
{
	class Window
	{
	public:
		Window();
		~Window();

		bool Initialise(WindowContainer* windowContainer, HINSTANCE hInstance, const char* windowTitle, std::string windowClass, int width, int height);
		bool ProcessMessages();
		HWND GetHWND() const;

	private:
		void RegisterWindowClass();

	private:
		HWND         mHandle = NULL;
		HINSTANCE    mHInstance = NULL;

		const char*  mWindowTitle = "";
		std::wstring mWindowTitleWide = L"";
		std::string  mWindowClass = "";
		std::wstring mWindowClassWide = L"";

		int          mWidth = 0;
		int          mHeight = 0;
	};
}
