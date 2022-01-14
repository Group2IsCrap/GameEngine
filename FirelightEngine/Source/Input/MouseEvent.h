#pragma once

class MouseEvent
{
public:
	struct MousePoint
	{
		int x;
		int y;
	};

	enum class e_MouseEventType
	{
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RawMove,
		Invalid
	};

	MouseEvent();
	MouseEvent(const e_MouseEventType type, const int x, const int y);
	~MouseEvent();

	bool IsValid();
	e_MouseEventType GetType();
	MousePoint GetPos();

	int GetMouseX();
	int GetMouseY();


public:
	e_MouseEventType m_Type;
	int m_XPos;
	int m_YPos;
};

