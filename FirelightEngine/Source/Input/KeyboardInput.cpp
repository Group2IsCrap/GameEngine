#include "KeyboardInput.h"

#include"KeyboardEvent.h"

namespace Firelight::Input
{
    KeyboardInput::KeyboardInput()
    {
        for (int i = 0; i < 256; i++)
        {
            m_Keystates[i] = false;
        }
    }

    bool KeyboardInput::KeyIsPress(const unsigned char key)
    {
        return m_Keystates[key];
    }

    bool KeyboardInput::KeyBufferIsEmpty()
    {
        return m_KeyBuffer.empty();
    }

    bool KeyboardInput::CharBufferIsEmpty()
    {
        return m_CharBuffer.empty();
    }

    KeyboardEvent KeyboardInput::ReadKey()
    {
        if (m_KeyBuffer.empty())
        {
            return KeyboardEvent();
        }
        else
        {
            KeyboardEvent event = m_KeyBuffer.front();
            m_KeyBuffer.pop();
            return event;
        }
    }

    unsigned char KeyboardInput::ReadChar()
    {
        if (m_CharBuffer.empty())
        {
            return 0u;
        }
        else
        {
            unsigned char event = m_CharBuffer.front();
            m_CharBuffer.pop();
            return event;
        }
    }

    void KeyboardInput::OnKeyPress(const unsigned char key)
    {
        m_Keystates[key] = true;
        m_KeyBuffer.push(KeyboardEvent(KeyboardEvent::KeyEvent::Press, key));
    }

    void KeyboardInput::OnKeyReplace(const unsigned char key)
    {
        m_Keystates[key] = false;
        m_KeyBuffer.push(KeyboardEvent(KeyboardEvent::KeyEvent::Release, key));
    }

    void KeyboardInput::OnChar(const unsigned char key)
    {
        m_CharBuffer.push(key);
    }

    void KeyboardInput::EnableAutoRepeatKeys()
    {
        m_AutoRepeatKeys = true;
    }

    void KeyboardInput::DisableAutoRepeatKeys()
    {
        m_AutoRepeatKeys = false;
    }

    void KeyboardInput::EnableAutoRepeatChars()
    {
        m_AutoRepeatChars = true;
    }

    void KeyboardInput::DisableAutoRepeatChars()
    {
        m_AutoRepeatChars = false;
    }

    bool KeyboardInput::IsCharAutoRepeat()
    {
        return m_AutoRepeatChars;
    }

    bool KeyboardInput::IsKeysAutoRepeat()
    {
        return m_AutoRepeatKeys;
    }
}