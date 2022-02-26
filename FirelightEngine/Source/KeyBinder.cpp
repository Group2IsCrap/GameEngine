#include "KeyBinder.h"
#include "Events/EventDispatcher.h"
#include "Input/KeyboardEvent.h"
#include "Input/ControllerEvent.h"

namespace Firelight
{
	KeyBinder::KeyBinder()
	{
		m_keyBinds = std::unordered_map<unsigned char, DescriptorType>();
		m_keyAxisBinds = std::unordered_map<DescriptorType, std::pair<unsigned char, float>>();

		Events::EventDispatcher::AddListener<Events::Input::OnKeyPress>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnKeyRelease>(this);
		Events::EventDispatcher::AddListener<Events::Input::KeyIsPressed>(this);
		Events::EventDispatcher::AddListener<Events::Input::ContollerEvent>(this);
	}

	void KeyBinder::BindKeyboardActionEvent(DescriptorType eventName, unsigned char key)
	{
		m_keyBinds[std::tolower(key)] = eventName;
	}

	void KeyBinder::BindKeyboardAxisEvent(DescriptorType eventName, unsigned char key, float axisValue)
	{
		m_keyAxisBinds[eventName] = std::make_pair(key, axisValue);
	}

	void KeyBinder::HandleEvents(DescriptorType event, void* data)
	{
		if (event == Events::Input::OnKeyPress::sm_descriptor)
		{
			RouteOnKeyPress(reinterpret_cast<unsigned char>(data));
		}
		else if (event == Events::Input::OnKeyRelease::sm_descriptor)
		{
			RouteOnKeyReleased(reinterpret_cast<unsigned char>(data));
		}
		else if (event == Events::Input::KeyIsPressed::sm_descriptor)
		{
			RouteKeyIsPressed(reinterpret_cast<unsigned char>(data));
		}
		else if (event == Events::Input::ContollerEvent::sm_descriptor)
		{
			RouteControllerEvent(data);
		}
	}

	void KeyBinder::RouteOnKeyPress(unsigned char pressedKey)
	{
		//if (m_keyBinds.find(pressedKey) != m_keyBinds.end())
		//{
		//	Events::EventDispatcher::InvokeFunctions(m_keyBinds[pressedKey]);
		//}
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
		if (m_keyBinds.find(pressedKey) != m_keyBinds.end())
		{
			Events::EventDispatcher::InvokeFunctions(m_keyBinds[pressedKey]);
		}
	}
	void KeyBinder::RouteControllerEvent(void* data)
	{

	}

}