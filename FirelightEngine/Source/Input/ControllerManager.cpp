#include "ControllerManager.h"
#include"ControllerInput.h"
namespace Firelight::Input {
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
		for (const auto& Con : m_Controller)
		{
			Con->ProcessInput();
		}

	}

	void ControllerManager::ProcessInput()
	{
		for (const auto& Con : m_Controller)
		{
			if (Con->m_Isconnected) {
				Con->TestInput();
			}
		}
		
	}
	ControllerInput* ControllerManager::GetController(int ContollerNo)
	{
		return m_Controller[ContollerNo].get();
	}
	void ControllerManager::AddContoller()
	{
		if (CanConnect()) {
			m_Controller.push_back(std::make_shared<ControllerInput>());
		}


		
	}
	void ControllerManager::AddContoller(int DeadZoneLeftThumb, int DeadZoneRightThumb, int TriggerThreshold)
	{
		if (CanConnect()) {
			m_NumberConnetedController++;
			m_Controller.push_back(std::make_shared<ControllerInput>(m_NumberConnetedController, DeadZoneLeftThumb, DeadZoneRightThumb, TriggerThreshold));
		}
	}
	void ControllerManager::RemoveController(int ToRemove)
	{
		//TODO Add Remove function
		m_Controller[ToRemove]->GetState();
	}
	bool ControllerManager::CanConnect()
	{
		return m_Controller.size() <=4;
	}
}