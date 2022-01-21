#pragma once

#include <vector>
#include <memory>

#include "Systems/System.h"

namespace Firelight::ECS
{
	class SystemManager
	{
	public:
		SystemManager();
		~SystemManager();

		void Update(double deltaTime);
		void FixedUpdate(double deltaTime);
		void LateUpdate(double deltaTime);

		void RegisterEngineSystems();
		
		template<typename SystemType>
		void RegisterGameSystem()
		{
			m_systems.push_back(std::make_unique<SystemType>());
		}

	private:
		std::vector<std::unique_ptr<System>> m_systems;
	};
}
