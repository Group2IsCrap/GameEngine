#pragma once
#include<Windows.h>
#include<Xinput.h>

#pragma comment(lib, "Xinput.lib")


/// <summary>
/// XBox 360 Controller Support
/// </summary>
class ControllerInput
{
public:
	ControllerInput();
	~ControllerInput();

	bool IsConnected();

	void ProcessInput();
	XINPUT_STATE GetState();

	void VibrateStop();
	void Vibrate(float leftVal, float rightVal);

	bool IsPressed(WORD buttion);
	bool isconnected=false;
private:
	int m_User_ID=0;

	int m_DeadzoneLeftThum=0;
	int m_DeadzoneRightThum=0;
	int m_TriggerThreshold=0;
	
	float leftTrigger=0;
	float rightTrigger = 0;

	float normLX = 0;
	float normLY = 0;

	float normRX = 0;
	float normRY = 0;

	XINPUT_STATE state;

};

