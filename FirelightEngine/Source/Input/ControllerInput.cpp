#include "ControllerInput.h"
#include <math.h>
#include<cstdio>
namespace Firelight::Input {
    ControllerInput::ControllerInput():
        m_User_ID(0),
        m_DeadzoneLeftThum(7849),
        m_DeadzoneRightThum(8689),
        m_TriggerThreshold(30)
    {
        m_State = XINPUT_STATE();
    }

    ControllerInput::ControllerInput(int ControllerIndex, double DeadZoneLeftThumb, double DeadZoneRightThumb, double TriggerThreshold):
        m_User_ID(ControllerIndex),
        m_DeadzoneLeftThum(DeadZoneLeftThumb),
        m_DeadzoneRightThum(DeadZoneRightThumb),
        m_TriggerThreshold(TriggerThreshold)
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

        ZeroMemory(&m_State, sizeof(XINPUT_STATE));

        //Simply get the state of the controller from XInput.
        dwResult = XInputGetState(m_User_ID, &m_State);

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

    //Translate into Non Xinput Data
    void ControllerInput::ProcessInput()
    {
        if (IsConnected()) {
            //trigger press amount between 0 and 1
            m_LeftTriggerPress = (float)m_State.Gamepad.bLeftTrigger / 255;
            m_RightTriggerPress = (float)m_State.Gamepad.bRightTrigger / 255;

            //dead zone right Thumb
            double RX = m_State.Gamepad.sThumbRX;
            double RY = m_State.Gamepad.sThumbRY;

            //determine how far the controller is pushed
            m_MagnitudeRightThum = sqrt(RX * RX + RY * RY);

            //determine the direction the controller is pushed
            m_NormalRightThumX = RX / m_MagnitudeRightThum;
            m_NormalRightThumY = RY / m_MagnitudeRightThum;


            //check if the controller is outside a circular dead zone
            if (m_MagnitudeRightThum > m_DeadzoneLeftThum)
            {
                //clip the magnitude at its expected maximum value
                if (m_MagnitudeRightThum > 32767) m_MagnitudeRightThum = 32767;

                //adjust magnitude relative to the end of the dead zone
                m_MagnitudeRightThum -= m_DeadzoneLeftThum;

                //optionally normalize the magnitude with respect to its expected range
                //giving a magnitude value of 0.0 to 1.0
                m_NormalizedRightThumbMagnitude = m_MagnitudeRightThum / (32767 - m_DeadzoneLeftThum);
            }
            else //if the controller is in the deadzone zero out the magnitude
            {
                m_MagnitudeRightThum = 0.0;
                m_NormalizedRightThumbMagnitude = 0.0;
            }

            //sticks movement between -1 and 1
            /*m_NormalRightThumX = fmaxf(-1, (double)state.Gamepad.sThumbRX / 32767);
            m_NormalRightThumY = fmaxf(-1, (double)state.Gamepad.sThumbRY / 32767);*/



            //dead zone left
            double LX = m_State.Gamepad.sThumbLX;
            double LY = m_State.Gamepad.sThumbLY;

            //determine how far the controller is pushed
            m_MagnitudeLeftThum = sqrt(LX * LX + LY * LY);

            //determine the direction the controller is pushed
            m_NormalLeftThumX = LX / m_MagnitudeLeftThum;
            m_NormalLeftThumY = LY / m_MagnitudeLeftThum;


            //check if the controller is outside a circular dead zone
            if (m_MagnitudeLeftThum > m_DeadzoneRightThum)
            {
                //clip the magnitude at its expected maximum value
                if (m_MagnitudeLeftThum > 32767) m_MagnitudeLeftThum = 32767;

                //adjust magnitude relative to the end of the dead zone
                m_MagnitudeLeftThum -= m_DeadzoneRightThum;

                //optionally normalize the magnitude with respect to its expected range
                //giving a magnitude value of 0.0 to 1.0
                m_NormalizedLeftThumbMagnitude = m_MagnitudeLeftThum / (32767 - m_DeadzoneRightThum);
            }
            else //if the controller is in the deadzone zero out the magnitude
            {
                m_MagnitudeLeftThum = 0.0;
                m_NormalizedLeftThumbMagnitude = 0.0;
            }

          /*  m_NormalLeftThumX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
            m_NormalLeftThumY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);*/
        }
       
    }

    //Directly get State from xinput
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


        WORD speedLeft = 65535 * leftVal;
        WORD speedRight = 65535 * rightVal;

        // Create a Vibraton State
        XINPUT_VIBRATION Vibration;

        // Zeroise the Vibration
        ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

        // use any value between 0 - 65535 here
        // Set the Vibration Values
        Vibration.wLeftMotorSpeed = speedLeft;
        Vibration.wRightMotorSpeed = speedRight;

        // Vibrate the controller
        XInputSetState(m_User_ID, &Vibration);
    }

    void ControllerInput::TestInput()
    {
        /*if (IsPressed(XINPUT_GAMEPAD_A)) {
            Vibrate(0.5, 0.5);
           
        }
        else if (IsPressed(XINPUT_GAMEPAD_DPAD_DOWN)) {
           
        }
        else
        {
            VibrateStop();
        }*/
       /* if (m_NormalizedLeftThumbMagnitude > 0.9f) {
           
                Vibrate(0.2f, 0.2f);
            
        }
        else
        {
            VibrateStop();
        }*/
    }

    bool ControllerInput::IsPressed(WORD buttion)
    {
        return (m_State.Gamepad.wButtons & buttion) != 0;
    }

    void ControllerInput::SetDeadZoneLeftThumb(double Deadzone)
    { 
        m_DeadzoneLeftThum = Deadzone;
    }

    void ControllerInput::SetDeadZoneRightThumb(double Deadzone)
    {
        m_DeadzoneRightThum = Deadzone;
    }

    void ControllerInput::SetTriggerThreshold(double TriggerThreshold)
    {
        m_TriggerThreshold = TriggerThreshold;
    }
  
   
}