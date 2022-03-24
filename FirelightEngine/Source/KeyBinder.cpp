#include "KeyBinder.h"
#include "Events/EventDispatcher.h"
#include "Input/KeyboardEvent.h"
#include "Input/ControllerEvent.h"

namespace Firelight
{
	KeyBinder::KeyBinder()
	{
		SetUpKeyMap();

		m_keyBinds = std::unordered_map<unsigned char, std::pair<DescriptorType,bool>>();
		m_keyAxisBinds = std::unordered_map<DescriptorType, std::pair<unsigned char, float>>();

		Events::EventDispatcher::AddListener<Events::Input::OnKeyPress>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnKeyRelease>(this);
		Events::EventDispatcher::AddListener<Events::Input::KeyIsPressed>(this);
		Events::EventDispatcher::AddListener<Events::Input::ContollerEvent>(this);

	}

	void KeyBinder::BindKeyboardActionEvent(DescriptorType eventName, unsigned char key)
	{
		m_keyBinds[key].first = eventName;
		m_keyBinds[key].second = false;
	}

	void KeyBinder::BindKeyboardActionEvent(DescriptorType eventName, Keys key)
	{
		unsigned char keyChar = m_keyMap[key];

		m_keyBinds[keyChar].first = eventName;
		m_keyBinds[keyChar].second = false;
	}

	void KeyBinder::BindKeyboardAxisEvent(DescriptorType eventName, unsigned char key, float axisValue)
	{
		m_keyAxisBinds[eventName] = std::make_pair(key, axisValue);
	}

	void KeyBinder::HandleEvents(DescriptorType event, void* data)
	{
		if (event == Events::Input::OnKeyPress::sm_descriptor)
		{
			if (m_keyBinds.find(reinterpret_cast<unsigned char>(data)) != m_keyBinds.end())
			{
				m_keyBinds.at(reinterpret_cast<unsigned char>(data)).second = true;
			}
			
		}
		else if (event == Events::Input::OnKeyRelease::sm_descriptor)
		{
			if (m_keyBinds.find(reinterpret_cast<unsigned char>(data)) != m_keyBinds.end())
			{
				m_keyBinds.at(reinterpret_cast<unsigned char>(data)).second = false;
			}
		}
		else if (event == Events::Input::ContollerEvent::sm_descriptor)
		{
			//RouteControllerEvent(data);

			//get contoller data
			//Firelight::Events::Input::ControllerState* eventController = (Firelight::Events::Input::ControllerState*)data;
			
		}
		
	}

	void KeyBinder::Update()
	{
		//done here to allow for rendring to to stutter
		CheckAllKeyOnPress(); 
	}

	void KeyBinder::CheckAllKeyOnPress()
	{
		for (auto& key: m_keyBinds)
		{
			if (key.second.second)
			{
				Events::EventDispatcher::InvokeFunctions(key.second.first);
			}
		}
	}

	void KeyBinder::RouteOnKeyPress(unsigned char pressedKey)
	{
		if (m_keyBinds.find(pressedKey) != m_keyBinds.end())
		{
			Events::EventDispatcher::InvokeFunctions(m_keyBinds[pressedKey].first);
		}
	}
	void KeyBinder::RouteOnKeyReleased(unsigned char pressedKey)
	{
		//if (m_keyBinds.find(pressedKey) != m_keyBinds.end())
		//{
		//	Events::EventDispatcher::InvokeFunctions(m_keyBinds[pressedKey]);
		//}
	}
	void KeyBinder::RouteKeyIsPressed(unsigned char pressedKey)
	{
		/*if (m_keyBinds.find(pressedKey) != m_keyBinds.end())
		{
			Events::EventDispatcher::InvokeFunctions(m_keyBinds[pressedKey]);
		}*/
	}
	void KeyBinder::RouteControllerEvent(void* data)
	{

	}

	void KeyBinder::SetUpKeyMap()
	{
		m_keyMap[Keys::KEY_A] = 0x41;
		m_keyMap[Keys::KEY_B] = 0x42;
		m_keyMap[Keys::KEY_C] = 0x43;
		m_keyMap[Keys::KEY_D] = 0x44;
		m_keyMap[Keys::KEY_E] = 0x45;
		m_keyMap[Keys::KEY_F] = 0x46;
		m_keyMap[Keys::KEY_G] = 0x47;
		m_keyMap[Keys::KEY_H] = 0x48;
		m_keyMap[Keys::KEY_I] = 0x49;
		m_keyMap[Keys::KEY_J] = 0x4A;
		m_keyMap[Keys::KEY_K] = 0x4B;
		m_keyMap[Keys::KEY_L] = 0x4C;
		m_keyMap[Keys::KEY_M] = 0x4D;
		m_keyMap[Keys::KEY_N] = 0x4E;
		m_keyMap[Keys::KEY_O] = 0x4F;
		m_keyMap[Keys::KEY_P] = 0x50;
		m_keyMap[Keys::KEY_Q] = 0x51;
		m_keyMap[Keys::KEY_R] = 0x52;
		m_keyMap[Keys::KEY_S] = 0x53;
		m_keyMap[Keys::KEY_T] = 0x54;
		m_keyMap[Keys::KEY_U] = 0x55;
		m_keyMap[Keys::KEY_V] = 0x56;
		m_keyMap[Keys::KEY_W] = 0x57;
		m_keyMap[Keys::KEY_X] = 0x58;
		m_keyMap[Keys::KEY_Y] = 0x59;
		m_keyMap[Keys::KEY_Z] = 0x5A;
		m_keyMap[Keys::KEY_0] = 0x30;
		m_keyMap[Keys::KEY_1] = 0x31;
		m_keyMap[Keys::KEY_2] = 0x32;
		m_keyMap[Keys::KEY_3] = 0x33;
		m_keyMap[Keys::KEY_4] = 0x34;
		m_keyMap[Keys::KEY_5] = 0x35;
		m_keyMap[Keys::KEY_6] = 0x36;
		m_keyMap[Keys::KEY_7] = 0x37;
		m_keyMap[Keys::KEY_8] = 0x38;
		m_keyMap[Keys::KEY_9] = 0x39;
		m_keyMap[Keys::KEY_LEFT_ARROW] = 37;
		m_keyMap[Keys::KEY_UP_ARROW] = 38;
		m_keyMap[Keys::KEY_RIGHT_ARROW] = 39 ;
		m_keyMap[Keys::KEY_DOWN_ARROW] = 40;
		m_keyMap[Keys::KEY_SHIFT] = 16;
		m_keyMap[Keys::KEY_CTRL] = 17;
		m_keyMap[Keys::KEY_ALT] = 18;
		m_keyMap[Keys::KEY_ESCAPE] = 27;
		m_keyMap[Keys::KEY_BACKSPACE] = 8;
		m_keyMap[Keys::KEY_RETURN] = 13;
		m_keyMap[Keys::KEY_COMMA] = 188;
		m_keyMap[Keys::KEY_PERIOD] = 190;
		m_keyMap[Keys::KEY_COLON] = 186;
		m_keyMap[Keys::KEY_APOSTROPHE] = 192;
		m_keyMap[Keys::KEY_HASH] = 222;
		m_keyMap[Keys::KEY_LEFT_SQUARE_BRACE] = 219;
		m_keyMap[Keys::KEY_RIGHT_SQUARE_BRACE] = 221;
		m_keyMap[Keys::KEY_CAPS_LOCK] = 20;
		m_keyMap[Keys::KEY_TAB] = 9;
		m_keyMap[Keys::KEY_PIPE] = 223;
		m_keyMap[Keys::KEY_BACKSLASH] = 220;
		m_keyMap[Keys::KEY_SLASH] = 191;
		m_keyMap[Keys::KEY_MINUS] = 189;
		m_keyMap[Keys::KEY_EQUALS] = 187;
		m_keyMap[Keys::KEY_INSERT] = 45;
		m_keyMap[Keys::KEY_DELETE] = 46;
		m_keyMap[Keys::KEY_HOME] = 36;
		m_keyMap[Keys::KEY_END] = 35;
		m_keyMap[Keys::KEY_PAGE_UP] = 33;
		m_keyMap[Keys::KEY_PAGE_DOWN] = 34;
		m_keyMap[Keys::KEY_PRINT_SCREEN] = 44;
		m_keyMap[Keys::KEY_SCROLL_LOCK] = 145;
		m_keyMap[Keys::KEY_NUM_LOCK] = 144;
		m_keyMap[Keys::KEY_PAUSE] = 19;
		m_keyMap[Keys::KEY_FUNCTION_1] = 112;
		m_keyMap[Keys::KEY_FUNCTION_2] = 113;
		m_keyMap[Keys::KEY_FUNCTION_3] = 114;
		m_keyMap[Keys::KEY_FUNCTION_4] = 115;
		m_keyMap[Keys::KEY_FUNCTION_5] = 116;
		m_keyMap[Keys::KEY_FUNCTION_6] = 117;
		m_keyMap[Keys::KEY_FUNCTION_7] = 118;
		m_keyMap[Keys::KEY_FUNCTION_8] = 119;
		m_keyMap[Keys::KEY_FUNCTION_9] = 120;
		m_keyMap[Keys::KEY_FUNCTION_10] = 121;
		m_keyMap[Keys::KEY_FUNCTION_11] = 122;
		m_keyMap[Keys::KEY_FUNCTION_12] = 123;
		m_keyMap[Keys::KEY_NUMPAD_0] = 96;
		m_keyMap[Keys::KEY_NUMPAD_1] = 97;
		m_keyMap[Keys::KEY_NUMPAD_2] = 98;
		m_keyMap[Keys::KEY_NUMPAD_3] = 99;
		m_keyMap[Keys::KEY_NUMPAD_4] = 100;
		m_keyMap[Keys::KEY_NUMPAD_5] = 101;
		m_keyMap[Keys::KEY_NUMPAD_6] = 102;
		m_keyMap[Keys::KEY_NUMPAD_7] = 103;
		m_keyMap[Keys::KEY_NUMPAD_8] = 104;
		m_keyMap[Keys::KEY_NUMPAD_9] = 105;
		m_keyMap[Keys::KEY_NUMPAD_RETURN] = 13;
		m_keyMap[Keys::KEY_NUMPAD_PERIOD] = 110;
		m_keyMap[Keys::KEY_NUMPAD_PLUS] = 107;
		m_keyMap[Keys::KEY_NUMPAD_MINUS] = 109;
		m_keyMap[Keys::KEY_NUMPAD_MULTIPLY] = 106;
		m_keyMap[Keys::KEY_NUMPAD_DIVIDE] = 111;
	}

}