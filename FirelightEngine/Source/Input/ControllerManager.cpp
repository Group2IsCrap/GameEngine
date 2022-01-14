#include "ControllerManager.h"
namespace Firelight::Input {
	ControllerManager::ControllerManager()
	{
		Controller.push_back(new ControllerInput());
	}

	ControllerManager::~ControllerManager()
	{
	}

	void ControllerManager::HandleInput()
	{

		Controller[0]->IsConnected();



	}

	void ControllerManager::ProcessInput()
	{
		if (Controller[0]->isconnected) {
			Controller[0]->ProcessInput();
		}
	}
}