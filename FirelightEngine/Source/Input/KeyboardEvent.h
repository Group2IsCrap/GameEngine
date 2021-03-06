#pragma once

#include"..\Events\Event.h"

namespace Firelight::Events::Input 
{
	NEW_EVENT(OnKeyPressNonRepeat);
	class KeyboardEvent: public Events::Event
	{
	public:
		enum class KeyEvent
		{
			Press,
			Release,
			Invalid
		};

	public:
		KeyboardEvent();
		KeyboardEvent(const KeyEvent type, const unsigned char key);

		bool IsPressed() const;
		bool IsReleased() const;
		bool IsValid() const;

		unsigned char GetKeyCode() const;
		
		static constexpr DescriptorType sm_descriptor = "KeyboardEvent";
		
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
		static constexpr DescriptorType sm_descriptor = "OnKeyPress";
	};

	
	class OnKeyRelease : public KeyboardEvent
	{
	public:
		OnKeyRelease();
		OnKeyRelease(const KeyEvent type, const unsigned char key);
		~OnKeyRelease();

		static constexpr DescriptorType sm_descriptor = "OnKeyRelease";
	};
	
	class KeyIsPressed : public KeyboardEvent
	{
	public:
		KeyIsPressed();
		KeyIsPressed(const KeyEvent type, const unsigned char key);
		~KeyIsPressed();

		static constexpr DescriptorType sm_descriptor = "KeyIsPressed";
	};
}
