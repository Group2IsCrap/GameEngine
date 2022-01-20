#include "ControllerInput.h"
#include <math.h>

namespace Firelight::Input
{
    ControllerInput::ControllerInput()
    {
    }

    ControllerInput::~ControllerInput()
    {
    }

    // Check if controllor is connected
    bool ControllerInput::IsConnected()
    {
        DWORD dwResult;

        ZeroMemory(&m_State, sizeof(XINPUT_STATE));

        //Simply get the state of the controller from XInput.
        dwResult = XInputGetState(m_User_ID, &m_State);

        if (dwResult == ERROR_SUCCESS)
        {
            isconnected = true;
            //trigger press amount between 0 and 1
            m_LeftTriggerPress = (double)m_State.Gamepad.bLeftTrigger / 255.0;
            m_RightTriggerPress = (double)m_State.Gamepad.bRightTrigger / 255.0;

            //dead zone right Thumb
            double RX = (double)m_State.Gamepad.sThumbRX;
            double RY = (double)m_State.Gamepad.sThumbRY;

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
            m_NormalRightThumX = fmax(-1.0, (double)m_State.Gamepad.sThumbRX / 32767.0);
            m_NormalRightThumY = fmax(-1.0, (double)m_State.Gamepad.sThumbRY / 32767.0);

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
                if (m_MagnitudeLeftThum > 32767.0) m_MagnitudeLeftThum = 32767.0;

                //adjust magnitude relative to the end of the dead zone
                m_MagnitudeLeftThum -= m_DeadzoneRightThum;

                //optionally normalize the magnitude with respect to its expected range
                //giving a magnitude value of 0.0 to 1.0
                m_NormalizedLeftThumbMagnitude = m_MagnitudeLeftThum / (double)(32767 - m_DeadzoneRightThum);
            }
            else //if the controller is in the deadzone zero out the magnitude
            {
                m_MagnitudeLeftThum = 0.0;
                m_NormalizedLeftThumbMagnitude = 0.0;
            }

            m_NormalLeftThumX = fmax(-1.0, (double)m_State.Gamepad.sThumbLX / 32767.0);
            m_NormalLeftThumY = fmax(-1.0, (double)m_State.Gamepad.sThumbLY / 32767.0);

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
        if (IsPressed(XINPUT_GAMEPAD_A))
        {
            Vibrate(0.5, 0.5);
        }
        else if (IsPressed(XINPUT_GAMEPAD_DPAD_DOWN))
        {
            Vibrate(1, 1);
        }
        else
        {
            VibrateStop();
        }

        if (m_LeftTriggerPress >= 0.5)
        {
            Vibrate(0.5, 0.5);
        }

        if (m_NormalRightThumX == 1)
        {
            Vibrate(0.5, 0.5);
        }
        else if (m_NormalRightThumX == -1)
        {
            Vibrate(0.5, 0.5);
        }

        if (m_NormalRightThumY == 1)
        {
            Vibrate(0.5, 0.5);
        }
        else if (m_NormalRightThumY == -1)
        {
            Vibrate(0.5, 0.5);
        }
    }

    // Directly get State from xinput
    XINPUT_STATE ControllerInput::GetState()
    {
        if (IsConnected()) {

            XINPUT_STATE state;
            ZeroMemory(&state, sizeof(XINPUT_STATE));

            // Simply get the state of the controller from XInput.
            XInputGetState(m_User_ID, &state);
            return state;
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

        int speed = (int)(65535.0f * leftVal);

        // Create a Vibraton State
        XINPUT_VIBRATION Vibration;

        // Zeroise the Vibration
        ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

        // use any value between 0 - 65535 here
        // Set the Vibration Values
        Vibration.wLeftMotorSpeed = (WORD)speed;
        Vibration.wRightMotorSpeed = (WORD)speed;

        // Vibrate the controller
        XInputSetState(m_User_ID, &Vibration);
    }

    bool ControllerInput::IsPressed(WORD buttion)
    {
        return (m_State.Gamepad.wButtons & buttion) != 0;
    }
}