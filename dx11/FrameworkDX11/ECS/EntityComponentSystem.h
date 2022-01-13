#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"

#include <memory>

namespace Engine::ECS
{
	class EntityComponentSystem
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			m_componentManager->RegisterComponent<T>();
		}

		template<typename T>
		ComponentTypeID GetComponentType()
		{
			return m_componentManager->GetComponentType<T>();
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			m_componentManager->AddComponent<T>(entity, component);
			m_entityManager->UpdateEntitySignature(entity, m_componentManager->GetComponentType<T>(), true);
		}

		Entity CreateEntity()
		{
			return m_entityManager->CreateEntity();
		}

		void RemoveEntity(Entity entity)
		{
			m_entityManager->RemoveEntity(entity);

			//Component Manager Entity Removed
		}

		void DebugEntities();

		static EntityComponentSystem* Instance();
	private:
		EntityComponentSystem();
		~EntityComponentSystem() = default;
	private:
		static EntityComponentSystem* sm_instance;

		std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<ComponentManager> m_componentManager;
	};
}