#include "GetInput.h"

#include"MouseInput.h"
#include"KeyboardInput.h"
#include"ControllerInput.h"
#include"ControllerManager.h"
namespace Firelight::Input {
    Firelight::Input::GetInput::GetInput()
    {
    }

    GetInput::GetInput(std::shared_ptr<Input::MouseInput> m_MouseCaptuer, std::shared_ptr<Input::KeyboardInput> m_KeyboardCaptuer, std::shared_ptr<Input::ControllerManager> m_ControllerManager) :
        m_MouseCaptuer(m_MouseCaptuer),
        m_KeyboardCaptuer(m_KeyboardCaptuer),
        m_ControllerManager(m_ControllerManager)
    {       
    }

    Firelight::Input::GetInput::~GetInput()
    {
    }

    bool Firelight::Input::GetInput::KeyIsPress(const unsigned char key)
    {
        return m_KeyboardCaptuer->KeyIsPress(key);
    }

    bool GetInput::MouseButtionPress(e_MouseButtionPress MouseEvent)
    {

        switch (MouseEvent)
        {
        case e_MouseButtionPress::LeftButtionPress:
            return m_MouseCaptuer->IsLeftDown();
            break;
        case e_MouseButtionPress::RightButtionPress:
            return m_MouseCaptuer->IsRightDown();
            break;
       
        case e_MouseButtionPress::MiddleButtionPress:
            return m_MouseCaptuer->IsMiddleDown();
            break;
        default:
            break;
        }
       
        return false;
    }

    Maths::Vec2f GetInput::GetMousePos()
    {
        return Maths::Vec2f( m_MouseCaptuer->GetMousePosX(),m_MouseCaptuer->GetMousePosY());
    }

    Maths::Vec2f GetInput::GetMousePosRaw()
    {
        return Maths::Vec2f();
    }

    bool GetInput::IsButtionPress(e_ControllerButtion Buttion)
    {
        switch (Buttion)
        {
        case Firelight::Input::e_ControllerButtion::A:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_A);
            break;
        case Firelight::Input::e_ControllerButtion::Y:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_Y);
            break;
        case Firelight::Input::e_ControllerButtion::X:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_X);
            break;
        case Firelight::Input::e_ControllerButtion::B:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_B);
            break;
        case Firelight::Input::e_ControllerButtion::DPADUP:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_DPAD_UP);
            break;
        case Firelight::Input::e_ControllerButtion::DPADDOWN:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_DPAD_DOWN);
            break;
        case Firelight::Input::e_ControllerButtion::DPADLEFT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_DPAD_LEFT);
            break;
        case Firelight::Input::e_ControllerButtion::DPADRIGHT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT);
            break;
        case Firelight::Input::e_ControllerButtion::SHOULDERRIGHT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER);
            break;
        case Firelight::Input::e_ControllerButtion::SHOULDERLEFT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER);
            break;
        case Firelight::Input::e_ControllerButtion::STICKPRESSRIGHT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_RIGHT_THUMB);
            break;
        case Firelight::Input::e_ControllerButtion::STICKPRESSLEFT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_LEFT_THUMB);
            break;
        case Firelight::Input::e_ControllerButtion::START:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_START);
            break;
        case Firelight::Input::e_ControllerButtion::BACK:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_BACK);
            break;
        default:
            break;
        }
        return false;
    }

    Maths::Vec2f GetInput::ControllerStickMovement(e_ControllerSticks Stick)
    {
        switch (Stick)
        {
        case Firelight::Input::e_ControllerSticks::STICKLEFT:
            return m_ControllerManager->GetController(0)->GetLeftStickState();
            break;
        case Firelight::Input::e_ControllerSticks::STICKRIGHT:
            return m_ControllerManager->GetController(0)->GetRightStickState();
            break;
        default:
            break;
        }
        return Maths::Vec2f();
    }

    float GetInput::TriggerData(e_ControllerTrigger trigger)
    {
        switch (trigger)
        {
        case Firelight::Input::e_ControllerTrigger::LEFTTRIGGER:
            return m_ControllerManager->GetController(0)->GetLeftTriggerState();
            break;
        case Firelight::Input::e_ControllerTrigger::RIGHTTRIGGER:
            return m_ControllerManager->GetController(0)->GetRightTriggerState();
            break;
        default:
            break;
        }
        return 0.0f;
    }
    
    void GetInput::SetAutoRepeatKeys(bool isAutoRepaeat)
    {
        if (isAutoRepaeat) {
            m_KeyboardCaptuer->EnableAutoRepeatChars();
            m_KeyboardCaptuer->EnableAutoRepeatKeys();
        }
        else
        {
            m_KeyboardCaptuer->DisableAutoRepeatChars();
            m_KeyboardCaptuer->DisableAutoRepeatKeys();
        }
    }

    void GetInput::SetDeadZone(double DeadZoneRight,double DeadZoneLeft)
    {
        m_ControllerManager->GetController(0)->SetDeadZoneLeftThumb(DeadZoneLeft);
        m_ControllerManager->GetController(0)->SetDeadZoneRightThumb(DeadZoneRight);
    }

    void GetInput::SetTriggerThresHold(double SetTriggerThresHold)
    {
        m_ControllerManager->GetController(0)->SetTriggerThreshold(SetTriggerThresHold);
    }

    void GetInput::SetVibration(float leftVal, float rightVal)
    {
        m_ControllerManager->GetController(0)->Vibrate(leftVal, rightVal);
    }

    bool GetInput::Initialize(std::shared_ptr<Input::MouseInput> MouseCaptuer, std::shared_ptr<Input::KeyboardInput> KeyboardCaptuer, std::shared_ptr<Input::ControllerManager> ControllerManager)
    {
        m_MouseCaptuer= MouseCaptuer;
        m_KeyboardCaptuer = KeyboardCaptuer;
        m_ControllerManager = ControllerManager;

        return true;
    }
}