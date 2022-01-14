#include "MouseEvent.h"

MouseEvent::MouseEvent():
m_Type(e_MouseEventType::Invalid),
m_XPos(0),
m_YPos(0)
{
}

MouseEvent::MouseEvent(const e_MouseEventType type, const int x, const int y):
	m_Type(type),
	m_XPos(x),
	m_YPos(y)
{
}

MouseEvent::~MouseEvent()
{
}

bool MouseEvent::IsValid()
{
	return m_Type != e_MouseEventType::Invalid;
}

MouseEvent::e_MouseEventType MouseEvent::GetType()
{
	return m_Type;
}

MouseEvent::MousePoint MouseEvent::GetPos()
{
	return{m_XPos,m_YPos};
}

int MouseEvent::GetMouseX()
{
	return m_XPos;
}

int MouseEvent::GetMouseY()
{
	return m_YPos;
}
