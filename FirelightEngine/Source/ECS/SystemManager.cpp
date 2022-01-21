#include "SystemManager.h"

#include "Systems/SpriteRenderSystem.h"

namespace Firelight::ECS
{
	Firelight::ECS::SystemManager::SystemManager()
	{
	}

	SystemManager::~SystemManager()
	{
	}

	void SystemManager::Update(double deltaTime)
	{
		for (int systemIndex = 0; systemIndex < m_systems.size(); ++systemIndex)
		{
			m_systems[systemIndex]->Update(deltaTime);
		}
	}

	void SystemManager::FixedUpdate(double deltaTime)
	{
		for (int systemIndex = 0; systemIndex < m_systems.size(); ++systemIndex)
		{
			m_systems[systemIndex]->FixedUpdate(deltaTime);
		}
	}

	void SystemManager::LateUpdate(double deltaTime)
	{
		for (int systemIndex = 0; systemIndex < m_systems.size(); ++systemIndex)
		{
			m_systems[systemIndex]->LateUpdate(deltaTime);
		}
	}

	void SystemManager::RegisterEngineSystems()
	{
		m_systems.push_back(std::make_unique<SpriteRenderSystem>());
	}
}