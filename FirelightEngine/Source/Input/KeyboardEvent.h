#pragma once
#include"..\Events\Event.h"
namespace Firelight::Input {
	class KeyboardEvent: public Events::Event
	{
	public:
		enum class KeyEvent
		{
			press,
			release,
			Invalid

		};
		KeyboardEvent();
		KeyboardEvent(const KeyEvent type, const unsigned char key);

		bool isPressed() const;
		bool isReleased() const;
		bool isValid() const;
		unsigned char GetKeyCode() const;
		
		static constexpr DescriptorType sm_Des= "KeyboardEvent";

		virtual DescriptorType Type() const override
		{
			return sm_Des;
		}
	private:
		KeyEvent m_Type;
		unsigned char m_Key;
	};

}