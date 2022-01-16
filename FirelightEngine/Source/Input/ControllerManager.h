#pragma once
#include<vector>
#include<memory>
#include"ControllerInput.h"

namespace Firelight::Input {
	//class ControllerInput;
	class ControllerManager
	{
	public:
		ControllerManager();
		~ControllerManager();

		void HandleInput();
		void ProcessInput();

		ControllerInput* GetController(int ContollerNo);
		void AddContoller();
		void AddContoller(int DeadZoneLeftThumb, int DeadZoneRightThumb, int TriggerThreshold);
		void RemoveController(int ToRemove);
	private:
		bool CanConnect();
	private:
		std::vector<std::shared_ptr<ControllerInput>> m_Controller;
		int m_NumberConnetedController;
		
	};
}
