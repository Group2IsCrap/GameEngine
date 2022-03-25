#pragma once

#include<map>

namespace Firelight::Input 
{
	class KeyboardInput
	{
	public:
		KeyboardInput();

		bool KeyIsPress(const unsigned char key);
		bool KeyIsPressNoRepeat(const unsigned char key);
		void OnKeyPress(const unsigned char key);
		void OnKeyPressNonRepeat(const unsigned char key);

		void OnKeyRelease(const unsigned char key);

		void OnChar(const unsigned char key);
		void OnCharNoRepeat(const unsigned char key);
	private:
		//map of all keys pressed
		std::map<unsigned char,bool> m_Keystates;
		std::map<unsigned char, bool> m_KeystatesNonRepeat;
	};
}
