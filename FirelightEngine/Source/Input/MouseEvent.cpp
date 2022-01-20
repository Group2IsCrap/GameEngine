#include "MouseEvent.h"
namespace Firelight::Events::Input 
{
	MouseEvent::MouseEvent() :
		m_Type(e_MouseEventType::Invalid),
		m_Pos(0)
	{
	}

	MouseEvent::MouseEvent(const e_MouseEventType type, const int x, const int y) :
		m_Type(type),
		m_Pos(x, y)
	{
	}

	MouseEvent::~MouseEvent()
	{
	}

	bool MouseEvent::IsValid()
	{
		return m_Type != e_MouseEventType::Invalid;
	}

	Events::Input::e_MouseEventType MouseEvent::GetType()
	{
		return m_Type;
	}

	const Maths::Vec2i& MouseEvent::GetPos()
	{
		return m_Pos;
	}

	int MouseEvent::GetMouseX()
	{
		return m_Pos.x;
	}

	int MouseEvent::GetMouseY()
	{
		return m_Pos.y;
	}

	Input::MouseButtionReleaseEvent::MouseButtionReleaseEvent() :
		MouseEvent()
	{
	}

	Input::MouseButtionReleaseEvent::MouseButtionReleaseEvent(const e_MouseEventType type, const int x, const int y) :
		MouseEvent(type, x, y)
	{
	}

	Input::MouseButtionReleaseEvent::~MouseButtionReleaseEvent()
	{
	}

	Input::MouseButtionPressEvent::MouseButtionPressEvent()
	{
	}

	Input::MouseButtionPressEvent::MouseButtionPressEvent(const e_MouseEventType type, const int x, const int y) :
		MouseEvent(type, x, y)
	{
	}

	Input::MouseButtionPressEvent::~MouseButtionPressEvent()
	{
	}

	Input::MouseMoveEvent::MouseMoveEvent() :
		MouseEvent()
	{
	}

	Input::MouseMoveEvent::MouseMoveEvent(const e_MouseEventType type, const int x, const int y) :
		MouseEvent(type, x, y)
	{
	}

	Input::MouseMoveEvent::~MouseMoveEvent()
	{
	}

	Input::MouseMoveRawEvent::MouseMoveRawEvent() :
		MouseEvent()
	{
	}

	Input::MouseMoveRawEvent::MouseMoveRawEvent(const e_MouseEventType type, const int x, const int y) :
		MouseEvent(type, x, y)
	{
	}

	Input::MouseMoveRawEvent::~MouseMoveRawEvent()
	{
	}
}