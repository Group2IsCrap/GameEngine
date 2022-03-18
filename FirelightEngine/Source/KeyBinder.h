#pragma once

#include "Events/Event.h"
#include "Events/Listener.h"
#include "Utils/ErrorManager.h"

#include <vector>
#include<map>
#include <unordered_map>

namespace Firelight
{
	using DescriptorType = const char*;

	enum class Keys
	{
		KEY_A,
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
		KEY_0,
		KEY_1,
		KEY_2,
		KEY_3,
		KEY_4,
		KEY_5,
		KEY_6,
		KEY_7,
		KEY_8,
		KEY_9,
		KEY_LEFT_ARROW,
		KEY_UP_ARROW,
		KEY_RIGHT_ARROW,
		KEY_DOWN_ARROW,
		KEY_SHIFT,
		KEY_CTRL,
		KEY_ALT,
		KEY_ESCAPE,
		KEY_BACKSPACE,
		KEY_RETURN,
		KEY_COMMA,
		KEY_PERIOD,
		KEY_COLON,
		KEY_APOSTROPHE,
		KEY_HASH,
		KEY_LEFT_SQUARE_BRACE,
		KEY_RIGHT_SQUARE_BRACE,
		KEY_CAPS_LOCK,
		KEY_TAB,
		KEY_PIPE,
		KEY_BACKSLASH,
		KEY_SLASH,
		KEY_MINUS,
		KEY_EQUALS,
		KEY_INSERT,
		KEY_DELETE,
		KEY_HOME,
		KEY_END,
		KEY_PAGE_UP,
		KEY_PAGE_DOWN,
		KEY_PRINT_SCREEN,
		KEY_SCROLL_LOCK,
		KEY_NUM_LOCK,
		KEY_PAUSE,
		KEY_FUNCTION_1,
		KEY_FUNCTION_2,
		KEY_FUNCTION_3,
		KEY_FUNCTION_4,
		KEY_FUNCTION_5,
		KEY_FUNCTION_6,
		KEY_FUNCTION_7,
		KEY_FUNCTION_8,
		KEY_FUNCTION_9,
		KEY_FUNCTION_10,
		KEY_FUNCTION_11,
		KEY_FUNCTION_12,
		KEY_NUMPAD_0,
		KEY_NUMPAD_1,
		KEY_NUMPAD_2,
		KEY_NUMPAD_3,
		KEY_NUMPAD_4,
		KEY_NUMPAD_5,
		KEY_NUMPAD_6,
		KEY_NUMPAD_7,
		KEY_NUMPAD_8,
		KEY_NUMPAD_9,
		KEY_NUMPAD_RETURN, 
		KEY_NUMPAD_PERIOD,
		KEY_NUMPAD_PLUS,
		KEY_NUMPAD_MINUS,
		KEY_NUMPAD_MULTIPLY,
		KEY_NUMPAD_DIVIDE,
	};

	class KeyBinder : public Events::Listener
	{
	public:
		KeyBinder();
		void BindKeyboardActionEvent(DescriptorType eventName, unsigned char key);
		void BindKeyboardActionEvent(DescriptorType eventName, Keys key);
		void BindKeyboardAxisEvent(DescriptorType eventName, unsigned char key, float axisValue);

		virtual void HandleEvents(DescriptorType event, void* data) override;

		void Update();
	private:
		
		std::unordered_map<unsigned char,std::pair<DescriptorType,bool>> m_keyBinds;
		std::unordered_map<DescriptorType, std::pair<unsigned char, float>> m_keyAxisBinds;

		std::unordered_map<Keys, unsigned char> m_keyMap;

		void CheckAllKeyOnPress();

		void RouteOnKeyPress(unsigned char pressedKey);
		void RouteKeyIsPressed(unsigned char pressedKey);
		void RouteOnKeyReleased(unsigned char pressedKey);
		void RouteControllerEvent(void* data);
		void SetUpKeyMap();
	};
}

