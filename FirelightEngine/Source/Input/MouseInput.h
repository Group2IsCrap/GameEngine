#pragma once
#include<queue>
#include<Windows.h>

#include"MouseEvent.h"

namespace Firelight::Input {
	//class MouseEvent;
	class MouseInput
	{
	public:
		MouseInput();
		~MouseInput();

		void OnLeftPress(int x, int y);
		void OnLeftReleased(int x, int y);

		void OnRightPress(int x, int y);
		void OnRightReleased(int x, int y);

		void OnMiddlePress(int x, int y);
		void OnMiddleReleased(int x, int y);

		void OnWheelUp(int x, int y);
		void OnWheelDown(int x, int y);


		void OnMouseMove(int x, int y);
		void OnMouseMoveRaw(int x, int y);


		bool IsLeftDown();
		bool IsRightDown();
		bool IsMiddleDown();

		int GetMousePosX();
		int GetMousePosY();

		HRESULT RegisterMouseDevise();

		bool EventBufferIsEmpty();
		MouseEvent ReadEvent();

	private:
		std::queue<MouseEvent> m_MouseEventBuffer;

		bool m_IsLeftDown = false;
		bool m_IsRightDown = false;
		bool m_IsMiddleDown = false;
		int m_MousePosX = 0;
		int m_MousePosY = 0;


		bool m_RawInputInitialized = false;
	};

}