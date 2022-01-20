#pragma once

#include<vector>

#include "ControllerInput.h"

namespace Firelight::Input
{
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
}
