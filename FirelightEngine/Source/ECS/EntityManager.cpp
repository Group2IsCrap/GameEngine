#include "EntityManager.h"
#include <assert.h> 

namespace Firelight::ECS
{
	Entity EntityManager::sm_nextEntity = 0;

	Entity EntityManager::CreateEntity()
	{
		Entity entity = sm_nextEntity++;
		m_entities.push_back(entity);
		m_signatures.insert({ entity, Signature(3) });
		return entity;
	}

	void EntityManager::UpdateEntitySignature(Entity entity, ComponentTypeID componentID, bool contains)
	{
		m_signatures[entity][componentID] = contains;
	}


	void EntityManager::RemoveEntity(Entity entity)
	{
		const auto& it = std::find(m_entities.begin(), m_entities.end(), entity);

		assert(it != m_entities.end());

		// Entity was found so it can be removed
		m_signatures.erase(entity);
		m_entities.erase(it);
	}

	Signature EntityManager::GetEntitySignature(Entity entity)
	{
		return m_signatures[entity];
	}

	std::vector<Entity> EntityManager::GetEntities()
	{
		return m_entities;
	}

	//void EntityManager::DebugEntities()
	//{
	//	OutputDebugStringA("-------------------------------------\n");
	//	for (Entity entity : m_entities)
	//	{
	//		std::string output = "";
	//		Signature signature = m_signatures[entity];
	//		output += "Entity: " + std::to_string(entity) + ", Signature: ";
	//		for (auto bit : signature)
	//		{
	//			output += std::to_string(bit);
	//		} 
	//		output += "\n";
	//		OutputDebugStringA(output.c_str());
	//		//Loop components
	//	}
	//	OutputDebugStringA("-------------------------------------\n");
	//}
}