#pragma once

#include <queue>

#include"KeyboardEvent.h"

namespace Firelight::Input
{
	class KeyboardInput
	{
	public:
		KeyboardInput();
		bool KeyIsPress(const unsigned char key);
		bool KeyBufferIsEmpty();
		bool CharBufferIsEmpty();

		KeyboardEvent ReadKey();
		unsigned char ReadChar();

		void OnKeyPress(const unsigned char key);
		void OnKeyReplace(const unsigned char key);
		void OnChar(const unsigned char key);


		void EnableAutoRepeatKeys();
		void DisableAutoRepeatKeys();

		void EnableAutoRepeatChars();
		void DisableAutoRepeatChars();

		bool IsCharAutoRepeat();
		bool IsKeysAutoRepeat();

	private:
		bool m_AutoRepeatKeys = false;
		bool m_AutoRepeatChars = false;
		bool m_Keystates[256];

		std::queue<KeyboardEvent> m_KeyBuffer;
		std::queue<unsigned char> m_CharBuffer;
	};
}
