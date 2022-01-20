#include "MouseInput.h"
#include"MouseEvent.h"
#include"..\Events\EventDispatcher.h"
#include"..\Maths\Vec2.h"
namespace Firelight::Input {
    MouseInput::MouseInput()
    {
        m_RawInputInitialized = false;
        RegisterMouseDevise();
    }
    MouseInput::~MouseInput()
    {
    }
    void MouseInput::OnLeftPress(int x, int y)
    {
        m_IsLeftDown = true;

        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::LPress, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseButtionPressEvent(), (void*)Events::Input::e_MouseEventType::LPress);
    }

    void MouseInput::OnLeftReleased(int x, int y)
    {
        m_IsLeftDown = false;
        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::LRelease, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseButtionReleaseEvent(), (void*)Events::Input::e_MouseEventType::LRelease);
    }

    void MouseInput::OnRightPress(int x, int y)
    {
        m_IsRightDown = true;
        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::RPress, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseButtionPressEvent(), (void*)Events::Input::e_MouseEventType::RPress);
    }

    void MouseInput::OnRightReleased(int x, int y)
    {
        m_IsRightDown = false;
        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::RRelease, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseButtionReleaseEvent(), (void*)Events::Input::e_MouseEventType::RRelease);
    }

    void MouseInput::OnMiddlePress(int x, int y)
    {
        m_IsMiddleDown = true;
        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::MPress, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseButtionPressEvent(), (void*)Events::Input::e_MouseEventType::MPress);
    }

    void MouseInput::OnMiddleReleased(int x, int y)
    {
        m_IsMiddleDown = false;
        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::MRelease, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseButtionReleaseEvent(), (void*)Events::Input::e_MouseEventType::MRelease);
    }

    void MouseInput::OnWheelUp(int x, int y)
    {
        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::WheelUp, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseButtionPressEvent(), (void*)Events::Input::e_MouseEventType::WheelUp);
    }

    void MouseInput::OnWheelDown(int x, int y)
    {
        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::WheelDown, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseButtionPressEvent(), (void*)Events::Input::e_MouseEventType::WheelDown);
    }

    void MouseInput::OnMouseMove(int x, int y)
    {
        m_MousePosX = x;
        m_MousePosY = y;
        Maths::Vec2f Pos = Maths::Vec2f(m_MousePosX, m_MousePosY);

        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::Move, x, y));

        Events::EventDispatcher::InvokeListeners(Events::Input::MouseMoveEvent(), (void*)&Pos);
       
    }

    void MouseInput::OnMouseMoveRaw(int x, int y)
    {
        Maths::Vec2f Pos = Maths::Vec2f(x, y);
        //m_MouseEventBuffer.push(Events::Input::MouseEvent(Events::Input::e_MouseEventType::RawMove, x, y));
        Events::EventDispatcher::InvokeListeners(Events::Input::MouseMoveRawEvent(), (void*)&Pos);
        
    }

    bool MouseInput::IsLeftDown()
    {
        return m_IsLeftDown;
    }

    bool MouseInput::IsRightDown()
    {
        return m_IsRightDown;
    }

    bool MouseInput::IsMiddleDown()
    {
        return m_IsMiddleDown;
    }

    int MouseInput::GetMousePosX()
    {
        return m_MousePosX;
    }

    int MouseInput::GetMousePosY()
    {
        return m_MousePosY;
    }

    HRESULT MouseInput::RegisterMouseDevise()
    {
        if (m_RawInputInitialized == false)
        {
            RAWINPUTDEVICE rid;
            rid.usUsagePage = 0x01;
            rid.usUsage = 0x02;
            rid.hwndTarget = NULL;
            rid.dwFlags = 0;

            if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == false) {
                return E_FAIL;
            }
            return S_OK;
        }
        return E_FAIL;
    }

    bool MouseInput::EventBufferIsEmpty()
    {
        return m_MouseEventBuffer.empty();
    }

    Events::Input::MouseEvent MouseInput::ReadEvent()
    {
        if (m_MouseEventBuffer.empty()) {
            return Events::Input::MouseEvent();
        }
        else
        {
            Events::Input::MouseEvent Event = m_MouseEventBuffer.front();
            m_MouseEventBuffer.pop();
            return Event;
        }
    }
}