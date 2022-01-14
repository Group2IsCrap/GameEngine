#include "ControllerInput.h"
#include <math.h>


ControllerInput::ControllerInput()
{
}

ControllerInput::~ControllerInput()
{
}
//check if controllor is connected
bool ControllerInput::IsConnected()
{
    DWORD dwResult;

    ZeroMemory(&state, sizeof(XINPUT_STATE));

     //Simply get the state of the controller from XInput.
    dwResult = XInputGetState(m_User_ID, &state);

    if (dwResult == ERROR_SUCCESS)
    {
        isconnected = true;
        //trigger press amount between 0 and 1
        m_LeftTriggerPress = (float)state.Gamepad.bLeftTrigger / 255;
        m_RightTriggerPress = (float)state.Gamepad.bRightTrigger / 255;
       
         //dead zone right Thumb
        double RX = state.Gamepad.sThumbRX;
        double RY = state.Gamepad.sThumbRY;

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
         m_NormalRightThumX = fmaxf(-1, (double)state.Gamepad.sThumbRX / 32767);
         m_NormalRightThumY = fmaxf(-1, (double)state.Gamepad.sThumbRY / 32767);




         double LX = state.Gamepad.sThumbLX;
         double LY = state.Gamepad.sThumbLY;

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

         m_NormalLeftThumX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
         m_NormalLeftThumY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);

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
       
    if (IsPressed(XINPUT_GAMEPAD_A)) {
        Vibrate(0.5, 0.5);
    }
    else if (IsPressed(XINPUT_GAMEPAD_DPAD_DOWN)) {
        Vibrate(1, 1);
    }
    else
    {
        VibrateStop();
    }

    if (m_LeftTriggerPress >= 0.5) {
        Vibrate(0.5, 0.5);
    }

    if (m_NormalRightThumX == 1) {
        Vibrate(0.5, 0.5);
    }
    if (m_NormalRightThumX == -1) {
        Vibrate(0.5, 0.5);
    }

    if (m_NormalRightThumY == 1) {
        Vibrate(0.5, 0.5);
    }
    if (m_NormalRightThumY == -1) {
        Vibrate(0.5, 0.5);
    }
}

//Directly get State from xinput
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
    Vibrate(0,0);
}

void ControllerInput::Vibrate(float leftVal, float rightVal)
{


    int speed = 65535 * leftVal;


    // Create a Vibraton State
    XINPUT_VIBRATION Vibration;

    // Zeroise the Vibration
    ZeroMemory(&Vibration, sizeof(XINPUT_VIBRATION));

    // use any value between 0 - 65535 here
    // Set the Vibration Values
    Vibration.wLeftMotorSpeed = speed;
    Vibration.wRightMotorSpeed = speed;

    // Vibrate the controller
     XInputSetState(m_User_ID, &Vibration);
}

bool ControllerInput::IsPressed(WORD buttion)
{
    return (state.Gamepad.wButtons & buttion) != 0;
}
