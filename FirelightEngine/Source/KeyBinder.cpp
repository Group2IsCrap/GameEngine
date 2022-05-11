#include "KeyBinder.h"
#include "Events/EventDispatcher.h"
#include "Input/KeyboardEvent.h"

#include <map>

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
		CheckButton(eventType, button);
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

	void KeyBinder::BindControllerAxisEvent(DescriptorType eventName, ControllerThumbsticks stick)
	{
		switch (stick)
		{
		case ControllerThumbsticks::LEFT:
			m_leftStickGenericAxisBinds.push_back(eventName);
			break;
		case ControllerThumbsticks::RIGHT:
			m_rightStickGenericAxisBinds.push_back(eventName);
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
		CheckKey(eventType, key);
	}

	void KeyBinder::CheckKey(KeyEventType eventType, unsigned char key)
	{
		if (eventType == KeyEventType::KeyPressSingle)
		{
			if (!m_keySingleStates.contains(key))
			{
				m_keySingleStates[key] = false;
			}
		}
		else
		{
			if (!m_keyStates.contains(key))
			{
				m_keyStates[key] = false;
				m_previousKeyStates[key] = false;
			}
		}
	}
	
	void KeyBinder::CheckButton(ControllerEventType eventType, ControllerButtons button)
	{
		if (eventType == ControllerEventType::ButtonPressSingle)
		{
			if (!m_buttonSingleStates.contains(button))
			{
				m_buttonSingleStates[button] = false;
			}
		}
		else
		{
			if (!m_buttonStates.contains(button))
			{
				m_buttonStates[button] = false;
				m_previousButtonStates[button] = false;
			}
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
		for (auto& axisBind : m_leftStickGenericAxisBinds)
		{
			Events::EventDispatcher::InvokeListeners(axisBind, (void*)&m_leftStickAxis);
		}
		for (auto& axisBind : m_rightStickGenericAxisBinds)
		{
			Events::EventDispatcher::InvokeListeners(axisBind, (void*)&m_rightStickAxis);
		}

		m_previousButtonStates = m_buttonStates;
		m_previousKeyStates = m_keyStates;
	}

	std::vector<KeyboardBindingData> KeyBinder::GetCurrentKeyBindings()
	{
		std::map<std::string, KeyboardBindingData> bindingMap;
		std::vector<KeyboardBindingData> bindings;
		for (auto& binding : m_keyBindsOnPress)
		{
			KeyboardBindingData bindingData;
			bindingData.eventName = binding.second;
			bindingData.key = m_charMap[binding.first];
			bindingData.eventType = GetEnumString(KeyEventType::KeyPress);
			bindingMap[binding.second] = bindingData;
		}
		for (auto& binding : m_keyBindsOnPressSingle)
		{
			KeyboardBindingData bindingData;
			bindingData.eventName = binding.second;
			bindingData.key = m_charMap[binding.first];
			bindingData.eventType = GetEnumString(KeyEventType::KeyPressSingle);
			bindingMap[binding.second] = bindingData;
		}
		for (auto& binding : m_keyBindsOnRelease)
		{
			KeyboardBindingData bindingData;
			bindingData.eventName = binding.second;
			bindingData.key = m_charMap[binding.first];
			bindingData.eventType = GetEnumString(KeyEventType::KeyRelease);
			bindingMap[binding.second] = bindingData;
		}

		for (auto& binding : bindingMap)
		{
			bindings.push_back(binding.second);
		}

		return bindings;
	}

	std::vector<ControllerBindingData> KeyBinder::GetCurrentControllerBindings()
	{
		std::map<std::string, ControllerBindingData> bindingMap;
		std::vector<ControllerBindingData> bindings;
		for (auto& binding : m_controllerBindsOnPress)
		{
			ControllerBindingData bindingData;
			bindingData.eventName = binding.second;
			bindingData.button = binding.first;
			bindingData.eventType = GetEnumString(ControllerEventType::ButtonPress);
			bindingMap[binding.second] = bindingData;
		}
		for (auto& binding : m_controllerBindsOnPressSingle)
		{
			ControllerBindingData bindingData;
			bindingData.eventName = binding.second;
			bindingData.button = binding.first;
			bindingData.eventType = GetEnumString(ControllerEventType::ButtonPressSingle);
			bindingMap[binding.second] = bindingData;
		}
		for (auto& binding : m_controllerBindsOnRelease)
		{
			ControllerBindingData bindingData;
			bindingData.eventName = binding.second;
			bindingData.button = binding.first;
			bindingData.eventType = GetEnumString(ControllerEventType::ButtonRelease);
			bindingMap[binding.second] = bindingData;
		}

		for (auto& binding : bindingMap)
		{
			bindings.push_back(binding.second);
		}

		return bindings;
	}

	std::string KeyBinder::GetEnumString(KeyEventType eventType)
	{
		switch (eventType)
		{
		default:
		case KeyEventType::KeyPress:
			return "Key Press";
		case KeyEventType::KeyPressSingle:
			return "Key Press Single";
		case KeyEventType::KeyRelease:
			return "Key Release";
		}
	}

	KeyEventType KeyBinder::GetStringKeyEventTypeEnum(std::string eventType)
	{
		if (eventType == "Key Press")
		{
			return KeyEventType::KeyPress;
		}
		if (eventType == "Key Press Single")
		{
			return KeyEventType::KeyPressSingle;
		}
		if (eventType == "Key Release")
		{
			return KeyEventType::KeyRelease;
		}

		return KeyEventType::KeyPress;
	}

	std::string KeyBinder::GetEnumString(Keys key)
	{
		switch (key)
		{
		default:
		case Keys::KEY_A:
			return "A";
		case Keys::KEY_B:
			return "B";
		case Keys::KEY_C:
			return "C";
		case Keys::KEY_D:
			return "D";
		case Keys::KEY_E:
			return "E";
		case Keys::KEY_F:
			return "F";
		case Keys::KEY_G:
			return "G";
		case Keys::KEY_H:
			return "H";
		case Keys::KEY_I:
			return "I";
		case Keys::KEY_J:
			return "J";
		case Keys::KEY_K:
			return "K";
		case Keys::KEY_L:
			return "L";
		case Keys::KEY_M:
			return "M";
		case Keys::KEY_N:
			return "N";
		case Keys::KEY_O:
			return "O";
		case Keys::KEY_P:
			return "P";
		case Keys::KEY_Q:
			return "Q";
		case Keys::KEY_R:
			return "R";
		case Keys::KEY_S:
			return "S";
		case Keys::KEY_T:
			return "T";
		case Keys::KEY_U:
			return "U";
		case Keys::KEY_V:
			return "V";
		case Keys::KEY_W:
			return "W";
		case Keys::KEY_X:
			return "X";
		case Keys::KEY_Y:
			return "Y";
		case Keys::KEY_Z:
			return "Z";
		case Keys::KEY_0:
			return "0";
		case Keys::KEY_1:
			return "1";
		case Keys::KEY_2:
			return "2";
		case Keys::KEY_3:
			return "3";
		case Keys::KEY_4:
			return "4";
		case Keys::KEY_5:
			return "5";
		case Keys::KEY_6:
			return "6";
		case Keys::KEY_7:
			return "7";
		case Keys::KEY_8:
			return "8";
		case Keys::KEY_9:
			return "9";
		case Keys::KEY_FUNCTION_1:
			return "F1";
		case Keys::KEY_FUNCTION_2:
			return "F2";
		case Keys::KEY_FUNCTION_3:
			return "F3";
		case Keys::KEY_FUNCTION_4:
			return "F4";
		case Keys::KEY_FUNCTION_5:
			return "F5";
		case Keys::KEY_FUNCTION_6:
			return "F6";
		case Keys::KEY_FUNCTION_7:
			return "F7";
		case Keys::KEY_FUNCTION_8:
			return "F8";
		case Keys::KEY_FUNCTION_9:
			return "F9";
		case Keys::KEY_FUNCTION_10:
			return "F10";
		case Keys::KEY_FUNCTION_11:
			return "F11";
		case Keys::KEY_FUNCTION_12:
			return "F12";
		case Keys::KEY_COMMA:
			return ",";
		case Keys::KEY_PERIOD:
			return ".";
		case Keys::KEY_SLASH:
			return "/";
		case Keys::KEY_BACKSLASH:
			return "\\";
		case Keys::KEY_COLON:
			return ";";
		case Keys::KEY_APOSTROPHE:
			return "'";
		case Keys::KEY_HASH:
			return "#";
		case Keys::KEY_LEFT_SQUARE_BRACE:
			return "[";
		case Keys::KEY_RIGHT_SQUARE_BRACE:
			return "]";
		case Keys::KEY_MINUS:
			return "-";
		case Keys::KEY_EQUALS:
			return "=";
		case Keys::KEY_NUMPAD_0:
			return "NUMPAD 0";
		case Keys::KEY_NUMPAD_1:
			return "NUMPAD 1";
		case Keys::KEY_NUMPAD_2:
			return "NUMPAD 2";
		case Keys::KEY_NUMPAD_3:
			return "NUMPAD 3";
		case Keys::KEY_NUMPAD_4:
			return "NUMPAD 4";
		case Keys::KEY_NUMPAD_5:
			return "NUMPAD 5";
		case Keys::KEY_NUMPAD_6:
			return "NUMPAD 6";
		case Keys::KEY_NUMPAD_7:
			return "NUMPAD 7";
		case Keys::KEY_NUMPAD_8:
			return "NUMPAD 8";
		case Keys::KEY_NUMPAD_9:
			return "NUMPAD 9";
		case Keys::KEY_LEFT_ARROW:
			return "LEFT";
		case Keys::KEY_UP_ARROW:
			return "UP";
		case Keys::KEY_RIGHT_ARROW:
			return "RIGHT";
		case Keys::KEY_DOWN_ARROW:
			return "DOWN";
		}
	}

	Keys KeyBinder::GetStringKeyEnum(std::string key)
	{
		if (key == "A")
			return Keys::KEY_A;
		if (key == "B")
			return Keys::KEY_B;
		if (key == "C")
			return Keys::KEY_C;
		if (key == "D")
			return Keys::KEY_D;
		if (key == "E")
			return Keys::KEY_E;
		if (key == "F")
			return Keys::KEY_F;
		if (key == "G")
			return Keys::KEY_G;
		if (key == "H")
			return Keys::KEY_H;
		if (key == "I")
			return Keys::KEY_I;
		if (key == "J")
			return Keys::KEY_J;
		if (key == "K")
			return Keys::KEY_K;
		if (key == "L")
			return Keys::KEY_L;
		if (key == "M")
			return Keys::KEY_M;
		if (key == "N")
			return Keys::KEY_N;
		if (key == "O")
			return Keys::KEY_O;
		if (key == "P")
			return Keys::KEY_P;
		if (key == "Q")
			return Keys::KEY_Q;
		if (key == "R")
			return Keys::KEY_R;
		if (key == "S")
			return Keys::KEY_S;
		if (key == "T")
			return Keys::KEY_T;
		if (key == "U")
			return Keys::KEY_U;
		if (key == "V")
			return Keys::KEY_V;
		if (key == "W")
			return Keys::KEY_W;
		if (key == "X")
			return Keys::KEY_X;
		if (key == "Y")
			return Keys::KEY_Y;
		if (key == "Z")
			return Keys::KEY_Z;
		if (key == "0")
			return Keys::KEY_0;
		if (key == "1")
			return Keys::KEY_1;
		if (key == "2")
			return Keys::KEY_2;
		if (key == "3")
			return Keys::KEY_3;
		if (key == "4")
			return Keys::KEY_4;
		if (key == "5")
			return Keys::KEY_5;
		if (key == "6")
			return Keys::KEY_6;
		if (key == "7")
			return Keys::KEY_7;
		if (key == "8")
			return Keys::KEY_8;
		if (key == "9")
			return Keys::KEY_9;
		if (key == "F1")
			return Keys::KEY_FUNCTION_1;
		if (key == "F2")
			return Keys::KEY_FUNCTION_2;
		if (key == "F3")
			return Keys::KEY_FUNCTION_3;
		if (key == "F4")
			return Keys::KEY_FUNCTION_4;
		if (key == "F5")
			return Keys::KEY_FUNCTION_5;
		if (key == "F6")
			return Keys::KEY_FUNCTION_6;
		if (key == "F7")
			return Keys::KEY_FUNCTION_7;
		if (key == "F8")
			return Keys::KEY_FUNCTION_8;
		if (key == "F9")
			return Keys::KEY_FUNCTION_9;
		if (key == "F10")
			return Keys::KEY_FUNCTION_10;
		if (key == "F11")
			return Keys::KEY_FUNCTION_11;
		if (key == "F12")
			return Keys::KEY_FUNCTION_12;
		if (key == ",")
			return Keys::KEY_COMMA;
		if (key == ".")
			return Keys::KEY_PERIOD;
		if (key == "/")
			return Keys::KEY_SLASH;
		if (key == "\\")
			return Keys::KEY_BACKSLASH;
		if (key == ")")
			return Keys::KEY_COLON;
		if (key == "'")
			return Keys::KEY_APOSTROPHE;
		if (key == "#")
			return Keys::KEY_HASH;
		if (key == "[")
			return Keys::KEY_LEFT_SQUARE_BRACE;
		if (key == "]")
			return Keys::KEY_RIGHT_SQUARE_BRACE;
		if (key == "-")
			return Keys::KEY_MINUS;
		if (key == "==")
			return Keys::KEY_EQUALS;
		if (key == "NUMPAD 0")
			return Keys::KEY_NUMPAD_0;
		if (key == "NUMPAD 1")
			return Keys::KEY_NUMPAD_1;
		if (key == "NUMPAD 2")
			return Keys::KEY_NUMPAD_2;
		if (key == "NUMPAD 3")
			return Keys::KEY_NUMPAD_3;
		if (key == "NUMPAD 4")
			return Keys::KEY_NUMPAD_4;
		if (key == "NUMPAD 5")
			return Keys::KEY_NUMPAD_5;
		if (key == "NUMPAD 6")
			return Keys::KEY_NUMPAD_6;
		if (key == "NUMPAD 7")
			return Keys::KEY_NUMPAD_7;
		if (key == "NUMPAD 8")
			return Keys::KEY_NUMPAD_8;
		if (key == "NUMPAD 9")
			return Keys::KEY_NUMPAD_9;
		if (key == "LEFT")
			return Keys::KEY_LEFT_ARROW;
		if (key == "UP")
			return Keys::KEY_UP_ARROW;
		if (key == "RIGHT")
			return Keys::KEY_RIGHT_ARROW;
		if (key == "DOWN")
			return Keys::KEY_DOWN_ARROW;

		return Keys::KEY_A;
	}

	std::string KeyBinder::GetEnumString(ControllerEventType eventType)
	{
		switch (eventType)
		{
		default:
		case ControllerEventType::ButtonPress:
			return "Button Press";
		case ControllerEventType::ButtonPressSingle:
			return "Button Press Single";
		case ControllerEventType::ButtonRelease:
			return "Button Release";
		}
	}

	ControllerEventType KeyBinder::GetStringControllerEventTypeEnum(std::string eventType)
	{
		if (eventType == "Button Press")
		{
			return ControllerEventType::ButtonPress;
		}
		if (eventType == "Button Press Single")
		{
			return ControllerEventType::ButtonPressSingle;
		}
		if (eventType == "Button Release")
		{
			return ControllerEventType::ButtonRelease;
		}

		return ControllerEventType::ButtonPress;
	}

	std::string KeyBinder::GetEnumString(ControllerButtons button)
	{
		switch (button)
		{
		default:
		case ControllerButtons::A:
			return "A";
		case ControllerButtons::B:
			return "B";
		case ControllerButtons::X:
			return "X";
		case ControllerButtons::Y:
			return "Y";
		case ControllerButtons::STICK_LEFT:
			return "STICK LEFT";
		case ControllerButtons::STICK_RIGHT:
			return "STICK RIGHT";
		case ControllerButtons::START:
			return "START";
		case ControllerButtons::BACK:
			return "BACK";
		case ControllerButtons::LB:
			return "LB";
		case ControllerButtons::RB:
			return "RB";
		case ControllerButtons::LT:
			return "LT";
		case ControllerButtons::RT:
			return "RT";
		case ControllerButtons::DPAD_UP:
			return "UP";
		case ControllerButtons::DPAD_LEFT:
			return "LEFT";
		case ControllerButtons::DPAD_DOWN:
			return "DOWN";
		case ControllerButtons::DPAD_RIGHT:
			return "RIGHT";
		}
	}

	ControllerButtons KeyBinder::GetStringButtonEnum(std::string button)
	{
		if (button == "A")
			return ControllerButtons::A;
		if (button == "B")
			return ControllerButtons::B;
		if (button == "X")
			return ControllerButtons::X;
		if (button == "Y")
			return ControllerButtons::Y;
		if (button == "STICK LEFT")
			return ControllerButtons::STICK_LEFT;
		if (button == "STICK RIGHT")
			return ControllerButtons::STICK_RIGHT;
		if (button == "LB")
			return ControllerButtons::LB;
		if (button == "RB")
			return ControllerButtons::RB;
		if (button == "LT")
			return ControllerButtons::LT;
		if (button == "RT")
			return ControllerButtons::RT;
		if (button == "START")
			return ControllerButtons::START;
		if (button == "BACK")
			return ControllerButtons::BACK;
		if (button == "LEFT")
			return ControllerButtons::DPAD_LEFT;
		if (button == "UP")
			return ControllerButtons::DPAD_UP;
		if (button == "RIGHT")
			return ControllerButtons::DPAD_RIGHT;
		if (button == "DOWN")
			return ControllerButtons::DPAD_DOWN;

		return ControllerButtons::A;
	}

	void KeyBinder::ChangeKeyEventType(KeyboardBindingData binding, std::string newTypeString)
	{
		//Remove from current binding list
		KeyEventType currentType = GetStringKeyEventTypeEnum(binding.eventType);
		unsigned char keyChar = m_keyMap[binding.key];
		switch (currentType)
		{
		case KeyEventType::KeyPress:
			if (m_keyBindsOnPress.find(keyChar) != m_keyBindsOnPress.end())
			{
				m_keyBindsOnPress.erase(keyChar);
			}
			break;
		case KeyEventType::KeyPressSingle:
			if (m_keyBindsOnPressSingle.find(keyChar) != m_keyBindsOnPressSingle.end())
			{
				m_keyBindsOnPressSingle.erase(keyChar);
			}
			break;
		case KeyEventType::KeyRelease:
			if (m_keyBindsOnRelease.find(keyChar) != m_keyBindsOnRelease.end())
			{
				m_keyBindsOnRelease.erase(keyChar);
			}
			break;
		}

		//Add to new binding list
		KeyEventType newType = GetStringKeyEventTypeEnum(newTypeString);
		
		switch (newType)
		{
		case KeyEventType::KeyPress:
			m_keyBindsOnPress[keyChar] = binding.eventName;
			break;
		case KeyEventType::KeyPressSingle:
			m_keyBindsOnPressSingle[keyChar] = binding.eventName;
			break;
		case KeyEventType::KeyRelease:
			m_keyBindsOnRelease[keyChar] = binding.eventName;
			break;
		}
		
		CheckKey(newType, keyChar);
	}

	bool KeyBinder::ChangeEventKey(KeyboardBindingData binding, std::string newKeyString)
	{
		//Remove from current binding list
		Keys currentKey = binding.key;
		Keys newKey = GetStringKeyEnum(newKeyString);
		unsigned char currentKeyChar = m_keyMap[currentKey];
		unsigned char newKeyChar = m_keyMap[newKey];
		switch (GetStringKeyEventTypeEnum(binding.eventType))
		{
		case KeyEventType::KeyPress:
			if (m_keyBindsOnPress.find(newKeyChar) == m_keyBindsOnPress.end())
			{
				if (m_keyBindsOnPress.find(currentKeyChar) != m_keyBindsOnPress.end())
				{
					m_keyBindsOnPress.erase(currentKeyChar);
				}
				m_keyBindsOnPress[newKeyChar] = binding.eventName;
			}
			else
			{
				return false;
			}
			break;
		case KeyEventType::KeyPressSingle:
			if (m_keyBindsOnPressSingle.find(newKeyChar) == m_keyBindsOnPressSingle.end())
			{
				if (m_keyBindsOnPressSingle.find(currentKeyChar) != m_keyBindsOnPressSingle.end())
				{
					m_keyBindsOnPressSingle.erase(currentKeyChar);
				}
				m_keyBindsOnPressSingle[newKeyChar] = binding.eventName;
			}
			else
			{
				return false;
			}
			break;
		case KeyEventType::KeyRelease:
			if (m_keyBindsOnRelease.find(newKeyChar) == m_keyBindsOnRelease.end())
			{
				if (m_keyBindsOnRelease.find(currentKeyChar) != m_keyBindsOnRelease.end())
				{
					m_keyBindsOnRelease.erase(currentKeyChar);
				}
				m_keyBindsOnRelease[newKeyChar] = binding.eventName;
			}
			else
			{
				return false;
			}
			break;
		}
		CheckKey(GetStringKeyEventTypeEnum(binding.eventType), newKeyChar);
		return true;
	}

	void KeyBinder::ChangeControllerEventType(ControllerBindingData binding, std::string newTypeString)
	{
		//Remove from current binding list
		ControllerEventType currentType = GetStringControllerEventTypeEnum(binding.eventType);
		switch (currentType)
		{
		case ControllerEventType::ButtonPress:
			if (m_controllerBindsOnPress.find(binding.button) != m_controllerBindsOnPress.end())
			{
				m_controllerBindsOnPress.erase(binding.button);
			}
			break;
		case ControllerEventType::ButtonPressSingle:
			if (m_controllerBindsOnPressSingle.find(binding.button) != m_controllerBindsOnPressSingle.end())
			{
				m_controllerBindsOnPressSingle.erase(binding.button);
			}
			break;
		case ControllerEventType::ButtonRelease:
			if (m_controllerBindsOnRelease.find(binding.button) != m_controllerBindsOnRelease.end())
			{
				m_controllerBindsOnRelease.erase(binding.button);
			}
			break;
		}

		//Add to new binding list
		ControllerEventType newType = GetStringControllerEventTypeEnum(newTypeString);

		switch (newType)
		{
		case ControllerEventType::ButtonPress:
			m_controllerBindsOnPress[binding.button] = binding.eventName;
			break;
		case ControllerEventType::ButtonPressSingle:
			m_controllerBindsOnPressSingle[binding.button] = binding.eventName;
			break;
		case ControllerEventType::ButtonRelease:
			m_controllerBindsOnRelease[binding.button] = binding.eventName;
			break;
		}

		CheckButton(newType, binding.button);
	}

	bool KeyBinder::ChangeEventButton(ControllerBindingData binding, std::string newButtonString)
	{
		//Remove from current binding list
		ControllerButtons currentButton = binding.button;
		ControllerButtons newButton = GetStringButtonEnum(newButtonString);
		switch (GetStringKeyEventTypeEnum(binding.eventType))
		{
		case KeyEventType::KeyPress:
			if (m_controllerBindsOnPress.find(newButton) == m_controllerBindsOnPress.end())
			{
				if (m_controllerBindsOnPress.find(currentButton) != m_controllerBindsOnPress.end())
				{
					m_controllerBindsOnPress.erase(currentButton);
				}
				m_controllerBindsOnPress[newButton] = binding.eventName;
			}
			else
			{
				return false;
			}
			break;
		case KeyEventType::KeyPressSingle:
			if (m_controllerBindsOnPressSingle.find(newButton) == m_controllerBindsOnPressSingle.end())
			{
				if (m_controllerBindsOnPressSingle.find(currentButton) != m_controllerBindsOnPressSingle.end())
				{
					m_controllerBindsOnPressSingle.erase(currentButton);
				}
				m_controllerBindsOnPressSingle[newButton] = binding.eventName;
			}
			else
			{
				return false;
			}
			break;
		case KeyEventType::KeyRelease:
			if (m_controllerBindsOnRelease.find(newButton) == m_controllerBindsOnRelease.end())
			{
				if (m_controllerBindsOnRelease.find(currentButton) != m_controllerBindsOnRelease.end())
				{
					m_controllerBindsOnRelease.erase(currentButton);
				}
				m_controllerBindsOnRelease[newButton] = binding.eventName;
			}
			else
			{
				return false;
			}
			break;
		}
		CheckButton(GetStringControllerEventTypeEnum(binding.eventType), newButton);
		return true;
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

		m_charMap[0x41] = Keys::KEY_A;					
		m_charMap[0x42] = Keys::KEY_B;					
		m_charMap[0x43] = Keys::KEY_C;					
		m_charMap[0x44] = Keys::KEY_D;					
		m_charMap[0x45] = Keys::KEY_E;					
		m_charMap[0x46] = Keys::KEY_F;					
		m_charMap[0x47] = Keys::KEY_G;					
		m_charMap[0x48] = Keys::KEY_H;					
		m_charMap[0x49] = Keys::KEY_I;					
		m_charMap[0x4A] = Keys::KEY_J;					
		m_charMap[0x4B] = Keys::KEY_K;					
		m_charMap[0x4C] = Keys::KEY_L;					
		m_charMap[0x4D] = Keys::KEY_M;					
		m_charMap[0x4E] = Keys::KEY_N;					
		m_charMap[0x4F] = Keys::KEY_O;					
		m_charMap[0x50] = Keys::KEY_P;					
		m_charMap[0x51] = Keys::KEY_Q;					
		m_charMap[0x52] = Keys::KEY_R;					
		m_charMap[0x53] = Keys::KEY_S;					
		m_charMap[0x54] = Keys::KEY_T;					
		m_charMap[0x55] = Keys::KEY_U;					
		m_charMap[0x56] = Keys::KEY_V;					
		m_charMap[0x57] = Keys::KEY_W;					
		m_charMap[0x58] = Keys::KEY_X;					
		m_charMap[0x59] = Keys::KEY_Y;					
		m_charMap[0x5A] = Keys::KEY_Z;					
		m_charMap[0x30] = Keys::KEY_0;					
		m_charMap[0x31] = Keys::KEY_1;					
		m_charMap[0x32] = Keys::KEY_2;					
		m_charMap[0x33] = Keys::KEY_3;					
		m_charMap[0x34] = Keys::KEY_4;					
		m_charMap[0x35] = Keys::KEY_5;					
		m_charMap[0x36] = Keys::KEY_6;					
		m_charMap[0x37] = Keys::KEY_7;					
		m_charMap[0x38] = Keys::KEY_8;					
		m_charMap[0x39] = Keys::KEY_9;					
		m_charMap[37] = Keys::KEY_LEFT_ARROW;			
		m_charMap[38] = Keys::KEY_UP_ARROW;				
		m_charMap[39] = Keys::KEY_RIGHT_ARROW;			
		m_charMap[40] = Keys::KEY_DOWN_ARROW;			
		m_charMap[16] = Keys::KEY_SHIFT;				
		m_charMap[17] = Keys::KEY_CTRL;					
		m_charMap[18] = Keys::KEY_ALT;					
		m_charMap[27] = Keys::KEY_ESCAPE;				
		m_charMap[8] = Keys::KEY_BACKSPACE;			
		m_charMap[13] = Keys::KEY_RETURN;				
		m_charMap[188] = Keys::KEY_COMMA;				
		m_charMap[190] = Keys::KEY_PERIOD;				
		m_charMap[186] = Keys::KEY_COLON;				
		m_charMap[192] = Keys::KEY_APOSTROPHE;			
		m_charMap[222] = Keys::KEY_HASH;					
		m_charMap[219] = Keys::KEY_LEFT_SQUARE_BRACE;	
		m_charMap[221] = Keys::KEY_RIGHT_SQUARE_BRACE;	
		m_charMap[20] = Keys::KEY_CAPS_LOCK;			
		m_charMap[9] = Keys::KEY_TAB;					
		m_charMap[223] = Keys::KEY_PIPE;					
		m_charMap[220] = Keys::KEY_BACKSLASH;			
		m_charMap[191] = Keys::KEY_SLASH;				
		m_charMap[189] = Keys::KEY_MINUS;				
		m_charMap[187] = Keys::KEY_EQUALS;				
		m_charMap[45] = Keys::KEY_INSERT;				
		m_charMap[46] = Keys::KEY_DELETE;				
		m_charMap[36] = Keys::KEY_HOME;					
		m_charMap[35] = Keys::KEY_END;					
		m_charMap[33] = Keys::KEY_PAGE_UP;				
		m_charMap[34] = Keys::KEY_PAGE_DOWN;			
		m_charMap[44] = Keys::KEY_PRINT_SCREEN;			
		m_charMap[145] = Keys::KEY_SCROLL_LOCK;			
		m_charMap[144] = Keys::KEY_NUM_LOCK;				
		m_charMap[19] = Keys::KEY_PAUSE;				
		m_charMap[112] = Keys::KEY_FUNCTION_1;			
		m_charMap[113] = Keys::KEY_FUNCTION_2;			
		m_charMap[114] = Keys::KEY_FUNCTION_3;			
		m_charMap[115] = Keys::KEY_FUNCTION_4;			
		m_charMap[116] = Keys::KEY_FUNCTION_5;			
		m_charMap[117] = Keys::KEY_FUNCTION_6;			
		m_charMap[118] = Keys::KEY_FUNCTION_7;			
		m_charMap[119] = Keys::KEY_FUNCTION_8;			
		m_charMap[120] = Keys::KEY_FUNCTION_9;			
		m_charMap[121] = Keys::KEY_FUNCTION_10;			
		m_charMap[122] = Keys::KEY_FUNCTION_11;			
		m_charMap[123] = Keys::KEY_FUNCTION_12;			
		m_charMap[96] = Keys::KEY_NUMPAD_0;				
		m_charMap[97] = Keys::KEY_NUMPAD_1;				
		m_charMap[98] = Keys::KEY_NUMPAD_2;				
		m_charMap[99] = Keys::KEY_NUMPAD_3;				
		m_charMap[100] = Keys::KEY_NUMPAD_4;				
		m_charMap[101] = Keys::KEY_NUMPAD_5;				
		m_charMap[102] = Keys::KEY_NUMPAD_6;				
		m_charMap[103] = Keys::KEY_NUMPAD_7;				
		m_charMap[104] = Keys::KEY_NUMPAD_8;				
		m_charMap[105] = Keys::KEY_NUMPAD_9;				
		m_charMap[13] = Keys::KEY_NUMPAD_RETURN;		
		m_charMap[110] = Keys::KEY_NUMPAD_PERIOD;		
		m_charMap[107] = Keys::KEY_NUMPAD_PLUS;			
		m_charMap[109] = Keys::KEY_NUMPAD_MINUS;			
		m_charMap[106] = Keys::KEY_NUMPAD_MULTIPLY;		
		m_charMap[111] = Keys::KEY_NUMPAD_DIVIDE;		
	}

}