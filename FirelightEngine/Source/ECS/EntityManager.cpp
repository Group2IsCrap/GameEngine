#include "EntityManager.h"
#include <assert.h> 

namespace Firelight::ECS
{
	Entity EntityManager::sm_nextEntity = 0;

	/// <summary>
	/// Creates a new entity with a unique ID
	/// </summary>
	/// <returns></returns>
	Entity EntityManager::CreateEntity()
	{
		Entity entity = sm_nextEntity++;
		m_entities.push_back(entity);
		// To Do: Make signature size update based on the number of components
		m_signatures.insert({ entity, Signature(3) });
		return entity;
	}

	/// <summary>
	/// Updates the given entities signature based on the component type given
	/// </summary>
	/// <param name="entity"></param>
	/// <param name="componentID"></param>
	/// <param name="contains"></param>
	void EntityManager::UpdateEntitySignature(Entity entity, ComponentTypeID componentID, bool contains)
	{
		m_signatures[entity][componentID] = contains;
	}

	/// <summary>
	/// Removes an entity
	/// </summary>
	/// <param name="entity"></param>
	void EntityManager::RemoveEntity(Entity entity)
	{
		const auto& it = std::find(m_entities.begin(), m_entities.end(), entity);

		assert(it != m_entities.end());

		// Entity was found so it can be removed
		m_signatures.erase(entity);
		m_entities.erase(it);
	}

	/// <summary>
	/// Returns the the given entities signature
	/// </summary>
	/// <param name="entity"></param>
	/// <returns></returns>
	Signature EntityManager::GetEntitySignature(Entity entity)
	{
		return m_signatures[entity];
	}

	/// <summary>
	/// Gets all entities
	/// </summary>
	/// <returns></returns>
	std::vector<Entity> EntityManager::GetEntities()
	{
		return m_entities;
	}
}