#include "KeyboardInput.h"
#include"KeyboardEvent.h"
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
    return m_CharBuff.empty();
}

KeyboardEvent KeyboardInput::ReadKey()
{
    if (m_KeyBuffer.empty()) {
        return KeyboardEvent();
    }
    else
    {
        KeyboardEvent event = m_KeyBuffer.front();
        m_KeyBuffer.pop();
        return event;
    }
}

unsigned char KeyboardInput::Raedchar()
{
    if (m_CharBuff.empty()) {
        return 0u;
    }
    else
    {
        unsigned char event = m_CharBuff.front();
        m_CharBuff.pop();
        return event;
    }
}

void KeyboardInput::OnKeyPress(const unsigned char key)
{
    m_Keystates[key] = true;
    m_KeyBuffer.push(KeyboardEvent(KeyboardEvent::KeyEvent::press, key));
}

void KeyboardInput::OnKeyRelace(const unsigned char key)
{
    m_Keystates[key] = false;
    m_KeyBuffer.push(KeyboardEvent(KeyboardEvent::KeyEvent::release, key));
}

void KeyboardInput::OnChar(const unsigned char key)
{
    m_CharBuff.push(key);
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

bool KeyboardInput::IsCharAutoRepat()
{
    return m_AutoRepeatChars;
}

bool KeyboardInput::IsKeysAutoRepat()
{
    return m_AutoRepeatKeys;
}
