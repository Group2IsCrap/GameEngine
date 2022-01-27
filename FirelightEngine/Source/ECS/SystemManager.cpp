#include "SystemManager.h"

#include "Systems/SpriteRenderSystem.h"
#include "Systems/AnimationSystem.h"
#include "../Source/Physics/PhysicsSystem.h"

namespace Firelight::ECS
{
	Firelight::ECS::SystemManager::SystemManager()
	{
	}

	SystemManager::~SystemManager()
	{
	}

	void SystemManager::Update(const Utils::Time& time)
	{
		for (int systemIndex = 0; systemIndex < m_systems.size(); ++systemIndex)
		{
			m_systems[systemIndex]->Update(time);
		}
	}

	void SystemManager::PhysicsUpdate(const Utils::Time& time)
	{
		for (int systemIndex = 0; systemIndex < m_systems.size(); ++systemIndex)
		{
			m_systems[systemIndex]->PhysicsUpdate(time);
		}
	}

	void SystemManager::LateUpdate(const Utils::Time& time)
	{
		for (int systemIndex = 0; systemIndex < m_systems.size(); ++systemIndex)
		{
			m_systems[systemIndex]->LateUpdate(time);
		}
	}

	void SystemManager::RegisterEngineSystems()
	{
		m_systems.push_back(std::make_unique<SpriteRenderSystem>());
		m_systems.push_back(std::make_unique<AnimationSystem>());
		m_systems.push_back(std::make_unique<Firelight::Physics::PhysicsSystem>());
	}
}
