#pragma once

#include<queue>

namespace Firelight::Input 
{
	class KeyboardInput
	{
	public:
		KeyboardInput();
		bool KeyIsPress(const unsigned char key);
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
		bool m_AutoRepeatKeys = true;
		bool m_AutoRepeatChars = false;
		bool m_Keystates[256];
	};
}
