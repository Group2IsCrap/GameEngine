#include "KeyboardInput.h"
#include"KeyboardEvent.h"
#include"..\Events\EventDispatcher.h"

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

    void KeyboardInput::OnKeyPress(const unsigned char key)
    {
        m_Keystates[key] = true;
        
        
        Events::EventDispatcher::InvokeListeners(Events::Input::OnKeyPress(), (void*)key);
        
    }

    void KeyboardInput::OnKeyRelace(const unsigned char key)
    {
        m_Keystates[key] = false;

        
        Events::EventDispatcher::InvokeListeners(Events::Input::OnKeyRelease(), (void*)key);
    }

    void KeyboardInput::OnChar(const unsigned char key)
    {
        //for a later created event if needed
        
    }

    //repeat keyinput
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
}