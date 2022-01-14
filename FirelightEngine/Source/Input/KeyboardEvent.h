#pragma once
class KeyboardEvent
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
private:
	KeyEvent m_Type;
	unsigned char m_Key;
};

