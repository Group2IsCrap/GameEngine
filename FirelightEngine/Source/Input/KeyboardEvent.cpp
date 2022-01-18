#include "KeyboardEvent.h"

namespace Firelight::Input {
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

    bool KeyboardEvent::isPressed() const
    {
        return m_Type == KeyEvent::press;
    }

    bool KeyboardEvent::isReleased() const
    {
        return m_Type == KeyEvent::release;
    }

    bool KeyboardEvent::isValid() const
    {
        return m_Type != KeyEvent::Invalid;
    }

    unsigned char KeyboardEvent::GetKeyCode() const
    {
        return m_Key;
    }
}