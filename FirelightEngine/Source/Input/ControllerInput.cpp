#include "ControllerInput.h"

#include <math.h>
#include <cstdio>

#include "..\Events\EventDispatcher.h"
#include "ControllerEvent.h"

namespace Firelight::Input 
{
    ControllerInput::ControllerInput() :
        m_User_ID(0),
        m_DeadzoneLeftThumb(7849),
        m_DeadzoneRightThumb(8689),
        m_TriggerThreshold(30)
    {
        m_State = XINPUT_STATE();
    }

    ControllerInput::ControllerInput(int controllerIndex, double deadZoneLeftThumb, double deadZoneRightThumb, double triggerThreshold) :
        m_User_ID(controllerIndex),
        m_DeadzoneLeftThumb(deadZoneLeftThumb),
        m_DeadzoneRightThumb(deadZoneRightThumb),
        m_TriggerThreshold(triggerThreshold)
    {
        m_State = XINPUT_STATE();
    }

    ControllerInput::~ControllerInput()
    {
    }

    //check if controllor is connected and gets the state of the controller
    bool ControllerInput::IsConnected()
    {
        DWORD dwResult;
        XINPUT_STATE collectedState;
        ZeroMemory(&collectedState, sizeof(XINPUT_STATE));

        //Simply get the state of the controller from XInput.
        dwResult = XInputGetState(m_User_ID, &collectedState);

        if (collectedState.dwPacketNumber == m_State.dwPacketNumber)
        {
            return false;
        }
        else
        {
            ZeroMemory(&m_State, sizeof(XINPUT_STATE));
            m_State = collectedState;
        }

        if (dwResult == ERROR_SUCCESS)
        {
            m_Isconnected = true;

            return true;
        }
        else
        {
            return false;
        }
    }

    // Translate into Non Xinput Data
    void ControllerInput::ProcessInput()
    {
        if (IsConnected())
        {  
            Events::Input::ControllerState state;

            state.m_RightStick = GetRightStickState();
            state.m_LeftStick = GetLeftStickState();
            state.m_LeftTrigger = GetLeftTriggerState();
            state.m_RightTrigger = GetRightTriggerState(); 
            state.m_A = IsPressed(XINPUT_GAMEPAD_A);
            state.m_B = IsPressed(XINPUT_GAMEPAD_B);
            state.m_Y = IsPressed(XINPUT_GAMEPAD_Y);
            state.m_X = IsPressed(XINPUT_GAMEPAD_X);
            state.m_DPadUp = IsPressed(XINPUT_GAMEPAD_DPAD_UP);
            state.m_DPadDown = IsPressed(XINPUT_GAMEPAD_DPAD_DOWN);
            state.m_DPadLeft = IsPressed(XINPUT_GAMEPAD_DPAD_LEFT);
            state.m_DPadRight = IsPressed(XINPUT_GAMEPAD_DPAD_RIGHT);
            state.m_LeftBumber = IsPressed(XINPUT_GAMEPAD_LEFT_SHOULDER);
            state.m_RightBumber = IsPressed(XINPUT_GAMEPAD_RIGHT_SHOULDER);
            state.m_Back = IsPressed(XINPUT_GAMEPAD_BACK);
            state.m_Start = IsPressed(XINPUT_GAMEPAD_START);
            state.m_StickLeftPress = IsPressed(XINPUT_GAMEPAD_LEFT_THUMB);
            state.m_StickRightPress = IsPressed(XINPUT_GAMEPAD_RIGHT_THUMB);

            Events::EventDispatcher::InvokeListeners(Events::Input::ContollerEvent(), (void*)&state);
        }
    }

    // Directly get State from xinput
    XINPUT_STATE ControllerInput::GetState()
    {
        if (IsConnected()) {

            return m_State;
        }
        return XINPUT_STATE();
    }

    void ControllerInput::VibrateStop()
    {
        Vibrate(0, 0);
    }

    void ControllerInput::Vibrate(float leftVal, float rightVal)
    {
        (void)rightVal;

        WORD speedLeft = 65535 * leftVal;
        WORD speedRight = 65535 * rightVal;

        // Create a Vibraton State
        XINPUT_VIBRATION vibration;

        // Zeroise the Vibration
        ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

        // use any value between 0 - 65535 here
        // Set the Vibration Values
        vibration.wLeftMotorSpeed = speedLeft;
        vibration.wRightMotorSpeed = speedRight;

        // Vibrate the controller
        XInputSetState(m_User_ID, &vibration);
    }

    void ControllerInput::TestInput()
    {
       
    }

    bool ControllerInput::IsPressed(WORD buttion)
    {
        return (m_State.Gamepad.wButtons & buttion) != 0;
    }

    Maths::Vec2f ControllerInput::GetLeftStickState()
    { //dead zone left
        double lX = m_State.Gamepad.sThumbLX;
        double lY = m_State.Gamepad.sThumbLY;

        //determine how far the controller is pushed
        m_MagnitudeLeftThumb = sqrt(lX * lX + lY * lY);

        //determine the direction the controller is pushed
        m_NormalLeftThumbX = lX / m_MagnitudeLeftThumb;
        m_NormalLeftThumbY = lY / m_MagnitudeLeftThumb;


        //check if the controller is outside a circular dead zone
        if (m_MagnitudeLeftThumb > m_DeadzoneRightThumb)
        {
            //clip the magnitude at its expected maximum value
            if (m_MagnitudeLeftThumb > 32767) m_MagnitudeLeftThumb = 32767;

            //adjust magnitude relative to the end of the dead zone
            m_MagnitudeLeftThumb -= m_DeadzoneRightThumb;

            //optionally normalize the magnitude with respect to its expected range
            //giving a magnitude value of 0.0 to 1.0
            m_NormalizedLeftThumbMagnitude = m_MagnitudeLeftThumb / (32767 - m_DeadzoneRightThumb);
            return Maths::Vec2f((float)m_NormalLeftThumbX, (float)m_NormalLeftThumbY);
        }
        else //if the controller is in the deadzone zero out the magnitude
        {
           
            m_MagnitudeLeftThumb = 0.0;
            m_NormalizedLeftThumbMagnitude = 0.0; 
            return Maths::Vec2f(0, 0);
        }

       
        
    }

    Maths::Vec2f ControllerInput::GetRightStickState()
    {
        //dead zone right Thumb
        double rX = m_State.Gamepad.sThumbRX;
        double rY = m_State.Gamepad.sThumbRY;

        //determine how far the controller is pushed
        m_MagnitudeRightThumb = sqrt(rX * rX + rY * rY);

        //determine the direction the controller is pushed
        m_NormalRightThumbX = rX / m_MagnitudeRightThumb;
        m_NormalRightThumbY = rY / m_MagnitudeRightThumb;


        //check if the controller is outside a circular dead zone
        if (m_MagnitudeRightThumb > m_DeadzoneLeftThumb)
        {
            //clip the magnitude at its expected maximum value
            if (m_MagnitudeRightThumb > 32767) m_MagnitudeRightThumb = 32767;

            //adjust magnitude relative to the end of the dead zone
            m_MagnitudeRightThumb -= m_DeadzoneLeftThumb;

            //optionally normalize the magnitude with respect to its expected range
            //giving a magnitude value of 0.0 to 1.0
            m_NormalizedRightThumbMagnitude = m_MagnitudeRightThumb / (32767 - m_DeadzoneLeftThumb);

            return Maths::Vec2f((float)m_NormalRightThumbX, (float)m_NormalRightThumbY);
        }
        else //if the controller is in the deadzone zero out the magnitude
        {
           
            m_MagnitudeRightThumb = 0.0;
            m_NormalizedRightThumbMagnitude = 0.0; 
            return Maths::Vec2f(0,0);
        }
       
    }

    float ControllerInput::GetRightTriggerState()
    {
        if (m_State.Gamepad.bRightTrigger > m_TriggerThreshold) {
            m_RightTriggerPress = (float)m_State.Gamepad.bRightTrigger / 255;
           
        }
        else
        {
            m_RightTriggerPress = 0;
        }
        return (float)m_RightTriggerPress;
    }

    float ControllerInput::GetLeftTriggerState()
    {
        
        if (m_State.Gamepad.bLeftTrigger > m_TriggerThreshold) {
            m_LeftTriggerPress = (float)m_State.Gamepad.bLeftTrigger / 255;
        }
        else {
            m_LeftTriggerPress = 0;
        }
        return  (float)m_LeftTriggerPress;
      
    }

    void ControllerInput::SetDeadZoneLeftThumb(double deadzone)
    { 
        m_DeadzoneLeftThumb = deadzone;
    }

    void ControllerInput::SetDeadZoneRightThumb(double deadzone)
    {
        m_DeadzoneRightThumb = deadzone;
    }

    void ControllerInput::SetTriggerThreshold(double triggerThreshold)
    {
        m_TriggerThreshold = triggerThreshold;
    }
} 
