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

	class KeyBinder : public Events::Listener
	{
	public:
		KeyBinder();
		void BindKeyboardActionEvent(DescriptorType eventName, unsigned char key);
		void BindKeyboardAxisEvent(DescriptorType eventName, unsigned char key, float axisValue);

		virtual void HandleEvents(DescriptorType event, void* data) override;

		void Update();
	private:
		
		std::unordered_map<unsigned char,std::pair<DescriptorType,bool>> m_keyBinds;
		std::unordered_map<DescriptorType, std::pair<unsigned char, float>> m_keyAxisBinds;

		void CheckAllKeyOnPress();

		void RouteOnKeyPress(unsigned char pressedKey);
		void RouteKeyIsPressed(unsigned char pressedKey);
		void RouteOnKeyReleased(unsigned char pressedKey);
		void RouteControllerEvent(void* data);
	};
}

