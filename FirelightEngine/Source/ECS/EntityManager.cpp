#include "EntityManager.h"
#include "../Events/EventDispatcher.h"
#include "ECSEvents.h"

#include <assert.h> 

namespace Firelight::ECS
{
	EntityID EntityManager::sm_nextEntity = 0;

	/// <summary>
	/// Creates a new entity with a unique ID
	/// </summary>
	/// <returns></returns>
	EntityID EntityManager::CreateEntity()
	{
		while (std::find(m_entities.begin(), m_entities.end(), sm_nextEntity) != m_entities.end())
		{
			sm_nextEntity++;
		}
		return CreateEntityInternal(sm_nextEntity++);
	}

	EntityID EntityManager::CreateEntity(EntityID id)
	{
		if (std::find(m_entities.begin(), m_entities.end(), id) != m_entities.end())
		{
			return id;
		}
		return CreateEntityInternal(id);
	}

	EntityID EntityManager::CreateEntityInternal(EntityID id)
	{
		EntityID entity = id;
		m_entities.push_back(entity);

		return entity;
	}

	/// <summary>
	/// Updates the given entities signature based on the component type given
	/// </summary>
	/// <param name="entity"></param>
	/// <param name="componentID"></param>
	/// <param name="contains"></param>
	void EntityManager::UpdateEntitySignature(EntityID entity, ComponentTypeID componentID, bool contains)
	{
		m_signatures[entity][componentID] = contains;
	}

	/// <summary>
	/// Creates a new signature for the entity that has a set number of components
	/// </summary>
	/// <param name="entity"></param>
	/// <param name="numComponents"></param>
	void EntityManager::CreateNewEntitySignature(EntityID entity, int numComponents)
	{
		m_signatures[entity] = Signature(numComponents);
	}

	void EntityManager::ClearSignatures()
	{
		m_signatures.clear();
	}

	/// <summary>
	/// Removes an entity
	/// </summary>
	/// <param name="entity"></param>
	void EntityManager::RemoveEntity(EntityID entity)
	{
		const auto& it = std::find(m_entities.begin(), m_entities.end(), entity);

		if (it == m_entities.end())
		{
			return;
		}

		// Entity was found so it can be removed
		m_signatures.erase(entity);
		m_entities.erase(it);

		Events::EventDispatcher::InvokeFunctions<Events::ECS::OnEntityDestroyedEvent>();
	}

	/// <summary>
	/// Returns the the given entities signature
	/// </summary>
	/// <param name="entity"></param>
	/// <returns></returns>
	Signature EntityManager::GetEntitySignature(EntityID entity)
	{
		return m_signatures[entity];
	}

	/// <summary>
	/// Gets all entities
	/// </summary>
	/// <returns></returns>
	std::vector<EntityID> EntityManager::GetEntities()
	{
		return m_entities;
	}

	EntityID EntityManager::sm_nextTemplate = 0;

	/// <summary>
	/// Creates a new template with a unique ID
	/// </summary>
	/// <returns></returns>
	EntityID EntityManager::CreateTemplate()
	{
		while (std::find(m_templates.begin(), m_templates.end(), sm_nextTemplate) != m_templates.end())
		{
			sm_nextTemplate++;
		}
		return CreateTemplateInternal(sm_nextTemplate++);
	}

	EntityID EntityManager::CreateTemplateInternal(EntityID id)
	{
		EntityID entityTemplate = id;
		m_templates.push_back(entityTemplate);

		return entityTemplate;
	}

	/// <summary>
	/// Removes a template
	/// </summary>
	/// <param name="entity"></param>
	void EntityManager::RemoveTemplate(EntityID enitityTemplate)
	{
		const auto& it = std::find(m_templates.begin(), m_templates.end(), enitityTemplate);

		if (it == m_templates.end())
		{
			return;
		}

		m_templates.erase(it);
	}

	/// <summary>
	/// Gets all entities
	/// </summary>
	/// <returns></returns>
	std::vector<EntityID> EntityManager::GetTemplates()
	{
		return m_templates;
	}
}