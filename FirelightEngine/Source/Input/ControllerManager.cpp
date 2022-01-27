#include "ControllerManager.h"

#include  "ControllerInput.h"

namespace Firelight::Input
{
	ControllerManager::ControllerManager()
	{
		m_NumberConnetedController = 0;
		m_Controller.push_back(std::make_shared<ControllerInput>());
	}

	ControllerManager::~ControllerManager()
	{
	}

	void ControllerManager::HandleInput()
	{
		for (const auto& controller : m_Controller)
		{
			controller->ProcessInput();
		}
		
	}

	void ControllerManager::ProcessInput()
	{
		for (const auto& controller : m_Controller)
		{
			if (controller->m_Isconnected) {
				controller->TestInput();
			}
		}
	}

	ControllerInput* ControllerManager::GetController(int contollerNo)
	{
		return m_Controller[contollerNo].get();
	}

	void ControllerManager::AddContoller()
	{
		if (CanConnect())
		{
			m_Controller.push_back(std::make_shared<ControllerInput>());
		}
	}

	void ControllerManager::AddContoller(int deadzoneLeftThumb, int deadzoneRightThumb, int triggerThreshold)
	{
		if (CanConnect())
		{
			m_NumberConnetedController++;
			m_Controller.push_back(std::make_shared<ControllerInput>(m_NumberConnetedController, deadzoneLeftThumb, deadzoneRightThumb, triggerThreshold));
		}
	}

	void ControllerManager::RemoveController(int toRemove)
	{
		//TODO Add Remove function
		m_Controller[toRemove]->GetState();
	}

	bool ControllerManager::CanConnect()
	{
		return m_Controller.size() <=4;
	}
}
