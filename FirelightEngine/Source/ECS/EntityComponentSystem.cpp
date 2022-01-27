#include "EntityComponentSystem.h"
#include "../Events/EventDispatcher.h"
#include "ECSEvents.h"

#include <windows.h> 
#include <debugapi.h> 
#include <iostream> 
#include <string> 

namespace Firelight::ECS
{
	EntityComponentSystem* EntityComponentSystem::sm_instance = nullptr;

	EntityComponentSystem* EntityComponentSystem::Instance()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new EntityComponentSystem();
		}
		return sm_instance;
	}

	EntityComponentSystem::EntityComponentSystem()
	{
		m_entityManager = std::make_unique<EntityManager>();
		m_componentManager = std::make_unique<ComponentManager>();

		Events::EventDispatcher::SubscribeFunction<Events::ECS::OnComponentRegisteredEvent>(std::bind(&EntityComponentSystem::UpdateAllEntitySignatures, this));
	}


	/// <summary>
	/// Creates a new entity
	/// </summary>
	/// <returns></returns>
	EntityID EntityComponentSystem::CreateEntity()
	{
		EntityID entity = m_entityManager->CreateEntity();

		m_entityManager->CreateNewEntitySignature(entity, m_componentManager->GetRegisteredComponentTypeCount());

		Events::EventDispatcher::InvokeFunctions<Events::ECS::OnEntityCreatedEvent>();

		return entity;
	}

	/// <summary>
	/// Creates a new entity with the given ID
	/// </summary>
	/// <param name="id"></param>
	/// <returns></returns>
	EntityID EntityComponentSystem::CreateEntity(EntityID id)
	{
		return m_entityManager->CreateEntity(id);
	}

	/// <summary>
	/// Get all entities
	/// </summary>
	/// <returns></returns>
	std::vector<EntityID> EntityComponentSystem::GetEntities()
	{
		return m_entityManager->GetEntities();
	}

	/// <summary>
	/// Get signature of specific entity
	/// </summary>
	/// <param name="entityID"></param>
	/// <returns></returns>
	Signature EntityComponentSystem::GetSignature(EntityID entityID)
	{
		return m_entityManager->GetEntitySignature(entityID);
	}


	/// <summary>
	/// Get total number of registered components
	/// </summary>
	/// <returns></returns>
	int EntityComponentSystem::GetRegisteredComponentTypeCount()
	{
		return m_componentManager->GetRegisteredComponentTypeCount();
	}

	/// <summary>
	/// Updates the signatures of all entities
	/// </summary>
	void EntityComponentSystem::UpdateAllEntitySignatures()
	{
		m_entityManager->ClearSignatures();

		int numComponents = m_componentManager->GetRegisteredComponentTypeCount();
		auto componentData = m_componentManager->GetComponentData();

		for (auto& entity : m_entityManager->GetEntities())
		{
			m_entityManager->CreateNewEntitySignature(entity, numComponents);

			for (auto componentType : componentData)
			{
				bool entityHasComponent = m_componentManager->HasComponent(componentType.first,entity);
				m_entityManager->UpdateEntitySignature(entity, componentType.first, entityHasComponent);
			}
		}
	}

	void EntityComponentSystem::Serialize()
	{

	}
}
