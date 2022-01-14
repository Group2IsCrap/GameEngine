#pragma once
#include<vector>
#include"ControllerInput.h"
//class ControllerInput;
class ControllerManager
{
public:
	ControllerManager();
	~ControllerManager();

	void HandleInput();
	void ProcessInput();
private:
	std::vector<ControllerInput*> Controller;
	

};

