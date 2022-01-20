#include "KeyboardEvent.h"

namespace Firelight::Input
{
    KeyboardEvent::KeyboardEvent() :
        m_Type(KeyEvent::Invalid),
        m_Key('A')
    {
    }

    KeyboardEvent::KeyboardEvent(const KeyEvent type, const unsigned char key) :
        m_Type(type),
        m_Key(key)
    {
    }

    bool KeyboardEvent::IsPressed() const
    {
        return m_Type == KeyEvent::Press;
    }

    bool KeyboardEvent::IsReleased() const
    {
        return m_Type == KeyEvent::Release;
    }

    bool KeyboardEvent::IsValid() const
    {
        return m_Type != KeyEvent::Invalid;
    }

    unsigned char KeyboardEvent::GetKeyCode() const
    {
        return m_Key;
    }
}
