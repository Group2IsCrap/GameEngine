#pragma once
#include<vector>
#include<memory>


namespace Firelight::Input 
{
	class ControllerInput;
	class ControllerManager
	{
	public:
		ControllerManager();
		~ControllerManager();

		void HandleInput();
		void ProcessInput();

		ControllerInput* GetController(int contollerNo);
		void AddContoller();
		void AddContoller(int deadzoneLeftThumb, int deadzoneRightThumb, int triggerThreshold);
		void RemoveController(int toRemove);
	private:
		bool CanConnect();
	private:
		std::vector<std::shared_ptr<ControllerInput>> m_Controller;
		int m_NumberConnetedController;
		
	};
}
