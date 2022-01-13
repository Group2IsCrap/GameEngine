#pragma once
#include <windows.h>
#include <windowsx.h>

/// <summary>
/// Base class for creating a window
/// used to store window values
/// </summary>
class Window
{
public:
	Window();
	~Window();
	//Creat window
	HRESULT InitWindow(HINSTANCE hInstance, int nCmdShow);
	//set window values
	void SetWindow(unsigned int Width, unsigned int Hight, LPCTSTR WindowName);


	int GetWidth() { return m_Width; }
	int GetHight() { return m_Hight; }
private:
	

public:
	HINSTANCE               m_hInst;
	HWND                    m_hWnd;

private:	
	
	unsigned int m_Width;
	unsigned int m_Hight;
	LPCTSTR m_WindowName;
};
LRESULT CALLBACK    WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);