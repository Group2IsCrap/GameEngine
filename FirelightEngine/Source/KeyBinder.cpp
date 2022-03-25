#include "KeyBinder.h"
#include "Events/EventDispatcher.h"
#include "Input/KeyboardEvent.h"

namespace Firelight
{
	KeyBinder::KeyBinder()
	{
		SetUpKeyMap();

		Events::EventDispatcher::AddListener<Events::Input::OnKeyPress>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnKeyPressNonRepeat>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnKeyRelease>(this);
		//Events::EventDispatcher::AddListener<Events::Input::KeyIsPressed>(this);
		//Events::EventDispatcher::AddListener<Events::Input::ControllerEvent>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnButtonPressed>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnButtonPressedSingle>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnButtonReleased>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnLeftThumbstickMoved>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnRightThumbstickMoved>(this);

	}

	void KeyBinder::BindKeyboardActionEvent(DescriptorType eventName, Keys key, KeyEventType eventType)
	{
		unsigned char keyChar = m_keyMap[key];
		BindKeyboardActionEvent(eventName, keyChar, eventType);
	}

	void KeyBinder::BindControllerActionEvent(DescriptorType eventName, ControllerButtons button, ControllerEventType eventType)
	{
		switch (eventType)
		{
		case ControllerEventType::ButtonPress:
			m_controllerBindsOnPress[button] = eventName;
			break;
		case ControllerEventType::ButtonPressSingle:
			m_controllerBindsOnPressSingle[button] = eventName;
			break;
		case ControllerEventType::ButtonRelease:
			m_controllerBindsOnRelease[button] = eventName;
			break;
		}

		if (!m_buttonStates.contains(button))
		{
			m_buttonStates[button] = false;
			m_previousButtonStates[button] = false;
			m_buttonSingleStates[button] = false;
		}
	}

	void KeyBinder::BindControllerAxisEvent(DescriptorType eventName, Axis axis, ControllerThumbsticks stick, float value)
	{
		if (value < 0)
		{
			value = -1;
		}
		if (value > 0)
		{
			value = 1;
		}

		switch (stick)
		{
		case ControllerThumbsticks::LEFT:
			m_leftStickAxisBinds[eventName] = std::make_pair(axis, value);
			break;
		case ControllerThumbsticks::RIGHT:
			m_rightStickAxisBinds[eventName] = std::make_pair(axis, value);
			break;
		}
	}

	void KeyBinder::BindKeyboardActionEvent(DescriptorType eventName, unsigned char key, KeyEventType eventType)
	{
		switch (eventType)
		{
		case KeyEventType::KeyPress:
			m_keyBindsOnPress[key] = eventName;
			break;
		case KeyEventType::KeyPressSingle:
			m_keyBindsOnPressSingle[key] = eventName;
			break;
		case KeyEventType::KeyRelease:
			m_keyBindsOnRelease[key] = eventName;
			break;
		}

		if (!m_keyStates.contains(key))
		{
			m_keyStates[key] = false;
			m_previousKeyStates[key] = false;
			m_keySingleStates[key] = false;
		}
	}


	void KeyBinder::HandleEvents(DescriptorType event, void* data)
	{
		if (event == Events::Input::OnKeyPress::sm_descriptor)
		{
			if (m_keyStates.find(reinterpret_cast<unsigned char>(data)) != m_keyStates.end())
			{
				m_keyStates.at(reinterpret_cast<unsigned char>(data)) = true;
			}
		}
		if (event == Events::Input::OnKeyPressNonRepeat::sm_descriptor)
		{
			if (m_keySingleStates.find(reinterpret_cast<unsigned char>(data)) != m_keySingleStates.end())
			{
				m_keySingleStates.at(reinterpret_cast<unsigned char>(data)) = true;
			}
		}
		if (event == Events::Input::OnKeyRelease::sm_descriptor)
		{
			if (m_keyStates.find(reinterpret_cast<unsigned char>(data)) != m_keyStates.end())
			{
				m_keyStates.at(reinterpret_cast<unsigned char>(data)) = false;
			}
		}
		if (event == Events::Input::OnButtonPressed::sm_descriptor)
		{
			if (m_buttonStates.find(static_cast<ControllerButtons>(reinterpret_cast<int>(data))) != m_buttonStates.end())
			{
				m_buttonStates.at(static_cast<ControllerButtons>(reinterpret_cast<int>(data))) = true;
			}

		}
		if (event == Events::Input::OnButtonPressedSingle::sm_descriptor)
		{
			if (m_buttonSingleStates.find(static_cast<ControllerButtons>(reinterpret_cast<int>(data))) != m_buttonSingleStates.end())
			{
				m_buttonSingleStates.at(static_cast<ControllerButtons>(reinterpret_cast<int>(data))) = true;
			}
		}
		if (event == Events::Input::OnButtonReleased::sm_descriptor)
		{
			if (m_buttonStates.find(static_cast<ControllerButtons>(reinterpret_cast<int>(data))) != m_buttonStates.end())
			{
				m_buttonStates.at(static_cast<ControllerButtons>(reinterpret_cast<int>(data))) = false;
			}
		}
		if (event == Events::Input::OnLeftThumbstickMoved::sm_descriptor)
		{
			m_leftStickAxis = *reinterpret_cast<Maths::Vec2f*>(data);
		}
		if (event == Events::Input::OnRightThumbstickMoved::sm_descriptor)
		{
			m_rightStickAxis = *reinterpret_cast<Maths::Vec2f*>(data);
		}
	}

	void KeyBinder::Update()
	{
		//done here to allow for rendering to to stutter

		for (auto& key : m_keySingleStates)
		{
			if (key.second)
			{
				if (m_keyBindsOnPressSingle.find(key.first) != m_keyBindsOnPressSingle.end())
				{
					Events::EventDispatcher::InvokeFunctions(m_keyBindsOnPressSingle[key.first]);
					key.second = false;
				}
			}
		}
		for (auto& key : m_keyStates)
		{
			if (key.second)
			{
				if (m_keyBindsOnPress.find(key.first) != m_keyBindsOnPress.end())
				{
					Events::EventDispatcher::InvokeFunctions(m_keyBindsOnPress[key.first]);
				}
			}
			else if (m_previousKeyStates[key.first])
			{
				if (m_keyBindsOnRelease.find(key.first) != m_keyBindsOnRelease.end())
				{
					Events::EventDispatcher::InvokeFunctions(m_keyBindsOnRelease[key.first]);
				}
			}
		}
		for (auto& button : m_buttonSingleStates)
		{
			if (button.second)
			{
				if (m_controllerBindsOnPressSingle.find(button.first) != m_controllerBindsOnPressSingle.end())
				{
					Events::EventDispatcher::InvokeFunctions(m_controllerBindsOnPressSingle[button.first]);
					button.second = false;
				}
			}
		}
		for (auto& button : m_buttonStates)
		{
			if (button.second)
			{
				if (m_controllerBindsOnPress.find(button.first) != m_controllerBindsOnPress.end())
				{
					Events::EventDispatcher::InvokeFunctions(m_controllerBindsOnPress[button.first]);
				}
			}
			else if (m_previousButtonStates[button.first])
			{
				if (m_controllerBindsOnRelease.find(button.first) != m_controllerBindsOnRelease.end())
				{
					Events::EventDispatcher::InvokeFunctions(m_controllerBindsOnRelease[button.first]);
				}
			}
		}
		for (auto& axisBind : m_leftStickAxisBinds)
		{
			float axisValue = axisBind.second.first == Axis::X ? m_leftStickAxis.x : m_leftStickAxis.y;
			if (axisBind.second.second < 0 && axisValue < -0.5 || axisBind.second.second > 0 && axisValue > 0.5)
			{
				Events::EventDispatcher::InvokeFunctions(axisBind.first);
			}
		}
		for (auto& axisBind : m_rightStickAxisBinds)
		{
			float axisValue = axisBind.second.first == Axis::X ? m_rightStickAxis.x : m_rightStickAxis.y;
			if (axisBind.second.second < 0 && axisValue < -0.5 || axisBind.second.second > 0 && axisValue > 0.5)
			{
				Events::EventDispatcher::InvokeFunctions(axisBind.first);
			}
		}

		m_previousButtonStates = m_buttonStates;
		m_previousKeyStates = m_keyStates;
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
		m_keyMap[Keys::KEY_RIGHT_ARROW] = 39;
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