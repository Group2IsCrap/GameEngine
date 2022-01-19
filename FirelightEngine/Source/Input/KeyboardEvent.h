#pragma once
#include"..\Events\Event.h"
namespace Firelight::Events::Input {
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
	protected:
		KeyEvent m_Type;
		unsigned char m_Key;
	};


	class OnKeyPress : public KeyboardEvent
	{
	public:
		OnKeyPress();
		OnKeyPress(const KeyEvent type, const unsigned char key);
		~OnKeyPress();
		static constexpr DescriptorType sm_Des = "OnKeyPress";

		virtual DescriptorType Type() const override
		{
			return sm_Des;
		}
	private:

	};

	
	class OnKeyRelease : public KeyboardEvent
	{
	public:
		OnKeyRelease();
		OnKeyRelease(const KeyEvent type, const unsigned char key);
		~OnKeyRelease();

		static constexpr DescriptorType sm_Des = "OnKeyRelease";

		virtual DescriptorType Type() const override
		{
			return sm_Des;
		}
	private:

	};
}