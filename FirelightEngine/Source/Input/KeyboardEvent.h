#pragma once
namespace Firelight::Input
{
	class KeyboardEvent
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

	private:
		KeyEvent m_Type;
		unsigned char m_Key;
	};
}
