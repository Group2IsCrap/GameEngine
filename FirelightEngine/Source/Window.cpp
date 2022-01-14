
//Function implementations for the RenderWindow class

#include "WindowContainer.h"

namespace Firelight
{
	Window::Window()
	{
	}

	Window::~Window()
	{
		if (m_handle != NULL)
		{
			UnregisterClass(m_windowClassWide.c_str(), m_hInstance);
			DestroyWindow(m_handle);
		}
	}

	bool Window::Initialise(WindowContainer* windowContainer, HINSTANCE hInstance, const char* windowTitle, std::string windowClass, int width, int height)
	{
		m_hInstance = hInstance;
		m_windowTitle = windowTitle;
		m_windowTitleWide = Utils::StringHelpers::StringToWide(windowTitle);
		m_windowClass = windowClass;
		m_windowClassWide = Utils::StringHelpers::StringToWide(windowClass);
		m_width = width;
		m_height = height;

		RegisterWindowClass();

		int screenCentreX = GetSystemMetrics(SM_CXSCREEN) / 2 - m_width / 2;
		int screenCentreY = GetSystemMetrics(SM_CYSCREEN) / 2 - m_height / 2;

		RECT windowRect;

		windowRect.left = screenCentreX;
		windowRect.top = screenCentreY;
		windowRect.right = windowRect.left + width;
		windowRect.bottom = windowRect.top + height;

		AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);

		m_handle = CreateWindowEx(0,                   // Default style
			m_windowClassWide.c_str(),
			m_windowTitleWide.c_str(),
			WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, // Window style
			windowRect.left,                          // X pos
			windowRect.top,                           // Y pos
			windowRect.right - windowRect.left,       // Width
			windowRect.bottom - windowRect.top,       // Height
			NULL,                                     // Parent window non existent
			NULL,                                     // Child window non existent
			m_hInstance,                               // Module instance for window
			windowContainer);

		if (m_handle == NULL)
		{
			COM_ERROR(GetLastError(), std::string("CreateWindowEX Failed for: ") + windowTitle);
			return false;
		}

		ShowWindow(m_handle, SW_SHOW);
		SetForegroundWindow(m_handle);
		SetFocus(m_handle);

		return true;
	}

	bool Window::ProcessMessages()
	{
		// Init message struct
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (PeekMessage(&msg, m_handle, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_NULL)
		{
			if (!IsWindow(m_handle))
			{
				m_handle = NULL;
				UnregisterClass(m_windowClassWide.c_str(), m_hInstance);
				return false;
			}
		}

		return true;
	}

	HWND Window::GetHWND() const
	{
		return m_handle;
	}

	LRESULT CALLBACK HandleMsgRedirect(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			return 0;

		default:
			WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_NCCREATE:
		{
			const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
			WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);

			ASSERT_FATAL(pWindow != nullptr, "Pointer to window container was null during WM_NCCREATE.");

			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
			SetWindowLongPtr(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
			return pWindow->WindowProc(hwnd, uMsg, wParam, lParam);
		}
		default:
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
		}
	}

	void Window::RegisterWindowClass()
	{
		WNDCLASSEX windowClass;
		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = HandleMessageSetup;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = m_hInstance;
		windowClass.hIcon = NULL;                          // No icon
		windowClass.hIconSm = NULL;                        // No Small icon
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW); // Default cursor
		windowClass.hbrBackground = NULL;                  // No background brush
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = m_windowClassWide.c_str();
		windowClass.cbSize = sizeof(WNDCLASSEX);
		RegisterClassEx(&windowClass);
	}
}
