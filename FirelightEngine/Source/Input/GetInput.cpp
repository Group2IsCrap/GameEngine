#include "GetInput.h"

#include"MouseInput.h"
#include"KeyboardInput.h"
#include"ControllerInput.h"
#include"ControllerManager.h"
namespace Firelight::Input 
{
    Firelight::Input::GetInput::GetInput()
    {
    }

    GetInput::GetInput(std::shared_ptr<Input::MouseInput> mouseCapture, std::shared_ptr<Input::KeyboardInput> keyboardCapture, std::shared_ptr<Input::ControllerManager> controllerManager) :
        m_MouseCapture(mouseCapture),
        m_KeyboardCapture(keyboardCapture),
        m_ControllerManager(controllerManager)
    {       
    }

    Firelight::Input::GetInput::~GetInput()
    {
    }

    bool Firelight::Input::GetInput::KeyIsPress(const unsigned char key)
    {
        return m_KeyboardCapture->KeyIsPress(key);
    }

    bool GetInput::MouseButtonPress(e_MouseButtonPress mouseButton)
    {

        switch (mouseButton)
        {
        case e_MouseButtonPress::LEFTBUTTONPRESS:
            return m_MouseCapture->IsLeftDown();
            break;
        case e_MouseButtonPress::RIGHTBUTTONPRESS:
            return m_MouseCapture->IsRightDown();
            break;
       
        case e_MouseButtonPress::MIDDLEBUTTONPRESS:
            return m_MouseCapture->IsMiddleDown();
            break;
        default:
            break;
        }
       
        return false;
    }

    Maths::Vec2i GetInput::GetMousePos()
    {
        return Maths::Vec2i(m_MouseCapture->GetMousePosX(),m_MouseCapture->GetMousePosY());
    }

    Maths::Vec2i GetInput::GetMousePosRaw()
    {
        return Maths::Vec2i();
    }

    bool GetInput::IsButtonPress(e_ControllerButton button)
    {
        switch (button)
        {
        case Firelight::Input::e_ControllerButton::A:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_A);
            break;
        case Firelight::Input::e_ControllerButton::Y:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_Y);
            break;
        case Firelight::Input::e_ControllerButton::X:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_X);
            break;
        case Firelight::Input::e_ControllerButton::B:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_B);
            break;
        case Firelight::Input::e_ControllerButton::DPADUP:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_DPAD_UP);
            break;
        case Firelight::Input::e_ControllerButton::DPADDOWN:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_DPAD_DOWN);
            break;
        case Firelight::Input::e_ControllerButton::DPADLEFT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_DPAD_LEFT);
            break;
        case Firelight::Input::e_ControllerButton::DPADRIGHT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT);
            break;
        case Firelight::Input::e_ControllerButton::SHOULDERRIGHT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER);
            break;
        case Firelight::Input::e_ControllerButton::SHOULDERLEFT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER);
            break;
        case Firelight::Input::e_ControllerButton::STICKPRESSRIGHT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_RIGHT_THUMB);
            break;
        case Firelight::Input::e_ControllerButton::STICKPRESSLEFT:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_LEFT_THUMB);
            break;
        case Firelight::Input::e_ControllerButton::START:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_START);
            break;
        case Firelight::Input::e_ControllerButton::BACK:
            return m_ControllerManager->GetController(0)->IsPressed(XINPUT_GAMEPAD_BACK);
            break;
        default:
            break;
        }
        return false;
    }

    Maths::Vec2f GetInput::ControllerStickMovement(e_ControllerSticks stick)
    {
        switch (stick)
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
    
    

    void GetInput::SetDeadZone(double deadzoneRight,double deadzoneLeft)
    {
        m_ControllerManager->GetController(0)->SetDeadZoneLeftThumb(deadzoneLeft);
        m_ControllerManager->GetController(0)->SetDeadZoneRightThumb(deadzoneRight);
    }

    void GetInput::SetTriggerThresHold(double setTriggerThreshold)
    {
        m_ControllerManager->GetController(0)->SetTriggerThreshold(setTriggerThreshold);
    }

    void GetInput::SetVibration(float leftVal, float rightVal)
    {
        m_ControllerManager->GetController(0)->Vibrate(leftVal, rightVal);
    }

    bool GetInput::Initialize(std::shared_ptr<Input::MouseInput> mouseCapture, std::shared_ptr<Input::KeyboardInput> KeyboardCapture, std::shared_ptr<Input::ControllerManager> controllerManager)
    {
        m_MouseCapture= mouseCapture;
        m_KeyboardCapture = KeyboardCapture;
        m_ControllerManager = controllerManager;

        return true;
    }
}