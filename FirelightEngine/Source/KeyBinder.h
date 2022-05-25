#pragma once

#include "Events/Event.h"
#include "Events/Listener.h"
#include "Utils/ErrorManager.h"

#include <vector>
#include<map>
#include <unordered_map>

#include "Input/ControllerEvent.h"

using namespace Firelight::Events::Input;

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
		KEY_SPACE,
		KEY_INVALID,
	};

	enum class KeyEventType
	{
		KeyPress,
		KeyPressSingle,
		KeyRelease
	};

	enum class ControllerEventType
	{
		ButtonPress,
		ButtonPressSingle,
		ButtonRelease
	};

	enum class ControllerThumbsticks
	{
		LEFT,
		RIGHT
	};

	enum class Axis
	{
		X,
		Y
	};

	struct KeyboardBindingData
	{
		std::string eventType;
		Keys key;
		DescriptorType eventName;
	};

	struct ControllerBindingData
	{
		std::string eventType;
		ControllerButtons button;
		DescriptorType eventName;
	};

	class KeyBinder : public Events::Listener
	{
	public:
		KeyBinder();
		void BindKeyboardActionEvent(DescriptorType eventName, unsigned char key, KeyEventType eventType = KeyEventType::KeyPress);
		void BindKeyboardActionEvent(DescriptorType eventName, Keys key, KeyEventType eventType = KeyEventType::KeyPress);
		void BindControllerActionEvent(DescriptorType eventName, ControllerButtons button, ControllerEventType eventType = ControllerEventType::ButtonPress);
		void BindControllerAxisEvent(DescriptorType eventName, Axis axis, ControllerThumbsticks stick, float value);

		void BindControllerAxisEvent(DescriptorType eventName, ControllerThumbsticks stick);


		void Update();

		std::vector<KeyboardBindingData> GetCurrentKeyBindings();
		std::vector<ControllerBindingData> GetCurrentControllerBindings();

		void ChangeKeyEventType(KeyboardBindingData binding, std::string newTypeString);
		bool ChangeEventKey(KeyboardBindingData binding, std::string newKeyString);
		void ChangeControllerEventType(ControllerBindingData binding, std::string newTypeString);
		bool ChangeEventButton(ControllerBindingData binding, std::string newButtonString);

		std::string GetEnumString(KeyEventType eventType);
		KeyEventType GetStringKeyEventTypeEnum(std::string eventType);
		std::string GetEnumString(Keys key);
		Keys GetStringKeyEnum(std::string key);
		std::string GetEnumString(ControllerEventType eventType);
		ControllerEventType GetStringControllerEventTypeEnum(std::string eventType);
		std::string GetEnumString(ControllerButtons button);
		ControllerButtons GetStringButtonEnum(std::string button);
	private:

		void CheckKey(KeyEventType eventType, unsigned char key);
		void CheckButton(ControllerEventType eventType, ControllerButtons button);
		virtual void HandleEvents(DescriptorType event, void* data) override;

		std::unordered_map<unsigned char, bool> m_keyStates;
		std::unordered_map<unsigned char, bool> m_previousKeyStates;
		std::unordered_map<unsigned char, bool> m_keySingleStates;
		std::unordered_map<ControllerButtons, bool> m_buttonStates;
		std::unordered_map<ControllerButtons, bool> m_previousButtonStates;
		std::unordered_map<ControllerButtons, bool> m_buttonSingleStates;

		Maths::Vec2f m_leftStickAxis = Maths::Vec2f(0,0);
		Maths::Vec2f m_rightStickAxis = Maths::Vec2f(0,0);

		std::unordered_map<unsigned char, DescriptorType> m_keyBindsOnPress;
		std::unordered_map<unsigned char, DescriptorType> m_keyBindsOnPressSingle;
		std::unordered_map<unsigned char, DescriptorType> m_keyBindsOnRelease;
		std::unordered_map<ControllerButtons, DescriptorType> m_controllerBindsOnPress;
		std::unordered_map<ControllerButtons, DescriptorType> m_controllerBindsOnPressSingle;
		std::unordered_map<ControllerButtons, DescriptorType> m_controllerBindsOnRelease;

		std::unordered_map<DescriptorType, std::pair<Axis, float>> m_leftStickAxisBinds;
		std::unordered_map<DescriptorType, std::pair<Axis, float>> m_rightStickAxisBinds;
		std::vector<DescriptorType> m_leftStickGenericAxisBinds;
		std::vector<DescriptorType> m_rightStickGenericAxisBinds;

		std::unordered_map<Keys, unsigned char> m_keyMap;
		std::unordered_map<unsigned char, Keys> m_charMap;

		void SetUpKeyMap();
	};
}

