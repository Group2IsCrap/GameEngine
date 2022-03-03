#pragma once

#include<queue>

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

		void OnKeyReplace(const unsigned char key);

		void OnChar(const unsigned char key);
		void OnCharNoRepeat(const unsigned char key);
	private:
		
		bool m_Keystates[256];
		bool m_KeystatesNonRepeat[256];
	};
}
