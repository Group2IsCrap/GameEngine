#include "EntityComponentSystem.h"
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
	}


	/// <summary>
	/// Creates a new entity
	/// </summary>
	/// <returns></returns>
	EntityID EntityComponentSystem::CreateEntity()
	{
		return m_entityManager->CreateEntity();
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
}
