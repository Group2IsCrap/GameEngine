#pragma once

#include <windows.h>
#include <windowsx.h>

#include "Maths/Vec2.h"

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

		bool Initialise(WindowContainer* windowContainer, HINSTANCE hInstance, const char* windowTitle, std::string windowClass, const Maths::Vec2i& dimensions);
		bool ProcessMessages();

		HWND GetHWND() const;
		void Destroy();

		const Maths::Vec2i& GetDimensions() const;
		const Maths::Vec2f& GetDimensionsFloat() const;

	private:
		void RegisterWindowClass();

	private:
		HWND         m_handle;
		HINSTANCE    m_hInstance;

		const char*  m_windowTitle;
		std::wstring m_windowTitleWide;
		std::string  m_windowClass;
		std::wstring m_windowClassWide;

		Maths::Vec2i m_dimensions;
		Maths::Vec2f m_floatDimensions;

		bool m_isDestroyed;
	};
}
