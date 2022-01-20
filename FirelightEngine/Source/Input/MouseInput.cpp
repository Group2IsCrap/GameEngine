#include "MouseInput.h"

#include "MouseEvent.h"

namespace Firelight::Input
{
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

        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::LPress, x, y));
    }

    void MouseInput::OnLeftReleased(int x, int y)
    {
        m_IsLeftDown = false;
        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::LRelease, x, y));
    }

    void MouseInput::OnRightPress(int x, int y)
    {
        m_IsRightDown = true;
        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::RPress, x, y));
    }

    void MouseInput::OnRightReleased(int x, int y)
    {
        m_IsRightDown = false;
        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::RRelease, x, y));
    }

    void MouseInput::OnMiddlePress(int x, int y)
    {
        m_IsMiddleDown = true;
        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::MPress, x, y));
    }

    void MouseInput::OnMiddleReleased(int x, int y)
    {
        m_IsMiddleDown = false;
        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::MRelease, x, y));
    }

    void MouseInput::OnWheelUp(int x, int y)
    {
        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::WheelUp, x, y));
    }

    void MouseInput::OnWheelDown(int x, int y)
    {
        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::WheelDown, x, y));
    }

    void MouseInput::OnMouseMove(int x, int y)
    {
        m_MousePosX = x;
        m_MousePosY = y;

        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::Move, x, y));
    }

    void MouseInput::OnMouseMoveRaw(int x, int y)
    {
        m_MouseEventBuffer.push(MouseEvent(MouseEvent::e_MouseEventType::RawMove, x, y));
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

    MouseEvent MouseInput::ReadEvent()
    {
        if (m_MouseEventBuffer.empty())
        {
            return MouseEvent();
        }
        else
        {
            MouseEvent Event = m_MouseEventBuffer.front();
            m_MouseEventBuffer.pop();
            return Event;
        }
    }
}