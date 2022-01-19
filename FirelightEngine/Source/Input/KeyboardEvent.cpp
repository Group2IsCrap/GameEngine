#include "KeyboardEvent.h"

namespace Firelight::Events::Input {
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

    Input::OnKeyPress::OnKeyPress()
    {
    }

    Input::OnKeyPress::OnKeyPress(const KeyEvent type, const unsigned char key):KeyboardEvent(type, key)
    {
    }
    Input::OnKeyPress::~OnKeyPress()
    {
    }
    Input::OnKeyRelease::OnKeyRelease()
    {
    }
    Input::OnKeyRelease::OnKeyRelease(const KeyEvent type, const unsigned char key):KeyboardEvent(type, key)
    {
    }
    Input::OnKeyRelease::~OnKeyRelease()
    {
    }
}