#include "KeyBinder.h"
#include "Events/EventDispatcher.h"
#include "Input/KeyboardEvent.h"
#include "Input/ControllerEvent.h"

namespace Firelight
{
	KeyBinder::KeyBinder()
	{
		m_keyBinds = std::unordered_map<unsigned char, std::pair<DescriptorType,bool>>();
		m_keyAxisBinds = std::unordered_map<DescriptorType, std::pair<unsigned char, float>>();

		Events::EventDispatcher::AddListener<Events::Input::OnKeyPress>(this);
		Events::EventDispatcher::AddListener<Events::Input::OnKeyRelease>(this);
		Events::EventDispatcher::AddListener<Events::Input::KeyIsPressed>(this);
		Events::EventDispatcher::AddListener<Events::Input::ContollerEvent>(this);

	}

	void KeyBinder::BindKeyboardActionEvent(DescriptorType eventName, unsigned char key)
	{
		m_keyBinds[std::tolower(key)].first = eventName;
		m_keyBinds[std::tolower(key)].second = false;
	}

	void KeyBinder::BindKeyboardAxisEvent(DescriptorType eventName, unsigned char key, float axisValue)
	{
		m_keyAxisBinds[eventName] = std::make_pair(key, axisValue);
	}

	void KeyBinder::HandleEvents(DescriptorType event, void* data)
	{
		if (event == Events::Input::OnKeyPress::sm_descriptor)
		{
			if (m_keyBinds.find(tolower(reinterpret_cast<unsigned char>(data))) != m_keyBinds.end())
			{
				m_keyBinds.at(tolower(reinterpret_cast<unsigned char>(data))).second = true;
			}
			
		}
		else if (event == Events::Input::OnKeyRelease::sm_descriptor)
		{
			if (m_keyBinds.find(tolower(reinterpret_cast<unsigned char>(data))) != m_keyBinds.end())
			{
				m_keyBinds.at(tolower(reinterpret_cast<unsigned char>(data))).second = false;
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
		for (auto key: m_keyBinds)
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

}