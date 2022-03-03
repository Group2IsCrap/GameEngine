#include "KeyboardInput.h"

#include "KeyboardEvent.h"

#include "..\Events\EventDispatcher.h"

namespace Firelight::Input
{
    KeyboardInput::KeyboardInput()
    {
        for (int i = 0; i < 256; i++)
        {
            m_Keystates[i] = false;
            m_KeystatesNonRepeat[i] = false;
        } 
    }

    bool KeyboardInput::KeyIsPress(const unsigned char key)
    {
        return m_Keystates[key];
    }

    bool KeyboardInput::KeyIsPressNoRepeat(const unsigned char key)
    {
        bool KeyIsPress = m_KeystatesNonRepeat[key];
        m_KeystatesNonRepeat[key] = false;
        return KeyIsPress;
    }

    void KeyboardInput::OnKeyPress(const unsigned char key)
    {
        m_Keystates[key] = true;
        Events::EventDispatcher::InvokeListeners<Events::Input::OnKeyPress>((void*)key);

    }
    void KeyboardInput::OnKeyPressNonRepeat(const unsigned char key)
    {
        m_KeystatesNonRepeat[key] = true;
        Events::EventDispatcher::InvokeListeners<Events::Input::OnKeyPressNonRepeat>((void*)key);
    }

    void KeyboardInput::OnKeyReplace(const unsigned char key)
    {
        m_Keystates[key] = false;
        m_KeystatesNonRepeat[key] = false;
        Events::EventDispatcher::InvokeListeners<Events::Input::OnKeyRelease>((void*)key);
    }

    void KeyboardInput::OnChar(const unsigned char key)
    {
        (void)key;

        //for a later created event if needed
    }

    void KeyboardInput::OnCharNoRepeat(const unsigned char key)
    {
    }

}
