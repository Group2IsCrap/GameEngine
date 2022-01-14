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
         leftTrigger = (float)state.Gamepad.bLeftTrigger / 255;
         rightTrigger = (float)state.Gamepad.bRightTrigger / 255;
       
         //dead zone
         float LX = state.Gamepad.sThumbLX;
         float LY = state.Gamepad.sThumbLY;

         //determine how far the controller is pushed
         float magnitude = sqrt(LX * LX + LY * LY);

         //determine the direction the controller is pushed
         float normalizedLX = LX / magnitude;
         float normalizedLY = LY / magnitude;

         float normalizedMagnitude = 0;

         //check if the controller is outside a circular dead zone
         if (magnitude > m_DeadzoneLeftThum)
         {
             //clip the magnitude at its expected maximum value
             if (magnitude > 32767) magnitude = 32767;

             //adjust magnitude relative to the end of the dead zone
             magnitude -= m_DeadzoneLeftThum;

             //optionally normalize the magnitude with respect to its expected range
             //giving a magnitude value of 0.0 to 1.0
             normalizedMagnitude = magnitude / (32767 - m_DeadzoneLeftThum);
         }
         else //if the controller is in the deadzone zero out the magnitude
         {
             magnitude = 0.0;
             normalizedMagnitude = 0.0;
         }

        //sticks movement between -1 and 1
         normLX = fmaxf(-1, (float)state.Gamepad.sThumbLX / 32767);
         normLY = fmaxf(-1, (float)state.Gamepad.sThumbLY / 32767);




         float RX = state.Gamepad.sThumbRX;
         float RY = state.Gamepad.sThumbRY;

         //determine how far the controller is pushed
         float magnitudeR = sqrt(RX * RX + RY * RY);

         //determine the direction the controller is pushed
         float normalizedRX = RX / magnitude;
         float normalizedRY = RY / magnitude;

         float normalizedMagnitudeR = 0;

         //check if the controller is outside a circular dead zone
         if (magnitude > m_DeadzoneRightThum)
         {
             //clip the magnitude at its expected maximum value
             if (magnitude > 32767) magnitude = 32767;

             //adjust magnitude relative to the end of the dead zone
             magnitude -= m_DeadzoneRightThum;

             //optionally normalize the magnitude with respect to its expected range
             //giving a magnitude value of 0.0 to 1.0
             normalizedMagnitude = magnitude / (32767 - m_DeadzoneRightThum);
         }
         else //if the controller is in the deadzone zero out the magnitude
         {
             magnitude = 0.0;
             normalizedMagnitude = 0.0;
         }



         normRX = fmaxf(-1, (float)state.Gamepad.sThumbRX / 32767);
         normRY = fmaxf(-1, (float)state.Gamepad.sThumbRY / 32767);
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

    if (leftTrigger >= 0.5) {
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
