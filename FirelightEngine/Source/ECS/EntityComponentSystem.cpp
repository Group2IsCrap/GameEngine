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

		Events::EventDispatcher::SubscribeFunction(Events::ECS::OnComponentRegisteredEvent::sm_descriptor, std::bind(&EntityComponentSystem::UpdateAllEntitySignatures, this));
	}


	/// <summary>
	/// Creates a new entity
	/// </summary>
	/// <returns></returns>
	EntityID EntityComponentSystem::CreateEntity()
	{
		EntityID entity = m_entityManager->CreateEntity();

		m_entityManager->CreateNewEntitySignature(entity, m_componentManager->GetComponentTypeCount());

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
	/// Outputs debug data on all entities
	/// </summary>
	void EntityComponentSystem::DebugEntities()
	{
		OutputDebugStringA("-------------------------------------\n");
		for (EntityID entity : m_entityManager->GetEntities())
		{
			std::string output = "";
			Signature signature = m_entityManager->GetEntitySignature(entity);
			output += "Entity: " + std::to_string(entity) + ", Signature: ";
			for (auto bit : signature)
			{
				output += std::to_string(bit);
			}
			output += "\n";
			OutputDebugStringA(output.c_str());
			for (auto& componentType : m_componentManager->GetComponentData())
			{
				output = "Component: ";
				if (signature[componentType.first])
				{
					output += m_componentManager->GetComponentName(componentType.first);
					output += "Value: ";
					BaseComponent* component = m_componentManager->GetComponent(componentType.first, entity);
					output += component->Output();
					output += "\n";
					OutputDebugStringA(output.c_str());
				}
			}
		}
		OutputDebugStringA("***********************************\n");
	}

	std::vector<EntityID> EntityComponentSystem::GetEntities()
	{
		return m_entityManager->GetEntities();
	}

	Signature EntityComponentSystem::GetSignature(EntityID entityID)
	{
		return m_entityManager->GetEntitySignature(entityID);
	}

	void EntityComponentSystem::UpdateAllEntitySignatures()
	{
		m_entityManager->ClearSignatures();

		int numComponents = m_componentManager->GetComponentTypeCount();
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
}
