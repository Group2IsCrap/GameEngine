#pragma once
#include<queue>
#include"KeyboardEvent.h"
namespace Firelight::Input {
	//class KeyboardEvent;
	class KeyboardInput
	{
	public:
		KeyboardInput();
		bool KeyIsPress(const unsigned char key);
		bool KeyBufferIsEmpty();
		bool CharBufferIsEmpty();

		Events::Input::KeyboardEvent ReadKey();
		unsigned char Raedchar();

		void OnKeyPress(const unsigned char key);
		void OnKeyRelace(const unsigned char key);
		void OnChar(const unsigned char key);


		void EnableAutoRepeatKeys();
		void DisableAutoRepeatKeys();

		void EnableAutoRepeatChars();
		void DisableAutoRepeatChars();

		bool IsCharAutoRepat();
		bool IsKeysAutoRepat();

	private:
		bool m_AutoRepeatKeys = true;
		bool m_AutoRepeatChars = false;
		bool m_Keystates[256];

		std::queue<Events::Input::KeyboardEvent> m_KeyBuffer;
		std::queue<unsigned char> m_CharBuff;
	};

}