#pragma once

#include <vector>
#include <memory>

#include "Systems/System.h"

#include "../Utils/Time.h"

namespace Firelight::ECS
{
	class SystemManager
	{
	public:
		SystemManager();
		~SystemManager();

		void FixedUpdate(const Utils::Time& time, const bool& isPaused);
		void Update(const Utils::Time& time, const bool& isPaused);
		void LateUpdate(const Utils::Time& time, const bool& isPaused);

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
