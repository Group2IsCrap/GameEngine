#include "System.h"

#include "../ECSEvents.h"
#include "../../Events/EventDispatcher.h"

namespace Firelight::ECS
{
	System::System()
	{
		m_onEntityCreatedIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnEntityCreatedEvent>(std::bind(&System::AddToEntityList, this, std::placeholders::_1));
		m_onEntityDestroyedIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnEntityDestroyedEvent>(std::bind(&System::RemoveFromEntityListWhenDestroyed, this, std::placeholders::_1));

		m_onComponentAddedIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnComponentAddedEvent>(std::bind(&System::AddToEntityList, this, std::placeholders::_1));
		m_onComponentRemovedIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnComponentRemovedEvent>(std::bind(&System::RemoveFromEntityList, this, std::placeholders::_1));

		m_onComponentRegisteredIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnComponentRegisteredEvent>(std::bind(&System::IncrementSignatureLists, this));

		m_whitelist = Signature(EntityComponentSystem::Instance()->GetRegisteredComponentTypeCount());
		m_blacklist = Signature(EntityComponentSystem::Instance()->GetRegisteredComponentTypeCount());
		m_entities = std::vector<Entity*>();
	}

	System::~System()
	{
		Events::EventDispatcher::UnsubscribeFunction<Events::ECS::OnEntityCreatedEvent>(m_onEntityCreatedIndex);
		Events::EventDispatcher::UnsubscribeFunction<Events::ECS::OnEntityDestroyedEvent>(m_onEntityDestroyedIndex);
		Events::EventDispatcher::UnsubscribeFunction<Events::ECS::OnComponentAddedEvent>(m_onComponentAddedIndex);
		Events::EventDispatcher::UnsubscribeFunction<Events::ECS::OnComponentRemovedEvent>(m_onComponentRemovedIndex);
		Events::EventDispatcher::UnsubscribeFunction<Events::ECS::OnComponentRegisteredEvent>(m_onComponentRegisteredIndex);

		for (auto entity : m_entities)
		{
			delete entity;
		}
	}

	/// <summary>
	/// Returns a list of the maintained entities
	/// </summary>
	/// <returns></returns>
	std::vector<Entity*> System::GetEntities()
	{
		return m_entities;
	}

	/// <summary>
	/// Physics update
	/// </summary>
	/// <param name="fixeddt"></param>
	void System::FixedUpdate(const Utils::Time& time, const bool& isPaused)
	{
		UNREFERENCED_PARAMETER(time);
		UNREFERENCED_PARAMETER(isPaused);
	}

	/// <summary>
	/// Primary update
	/// </summary>
	/// <param name="dt"></param>
	void System::Update(const Utils::Time& time, const bool& isPaused)
	{
		UNREFERENCED_PARAMETER(time);
		UNREFERENCED_PARAMETER(isPaused);
	}

	/// <summary>
	/// Secondary later update
	/// </summary>
	/// <param name="dt"></param>
	void System::LateUpdate(const Utils::Time& time, const bool& isPaused)
	{
		UNREFERENCED_PARAMETER(time);
		UNREFERENCED_PARAMETER(isPaused);
	}

	void System::AddToEntityList(void* data)
	{
		Entity* newEntity = new Entity((EntityID)data);

		// Do not re-add element
		if (IsEntityInList(newEntity->GetEntityID()))
		{
			return;
		}

		if (!IsValidEntity((EntityID)data))
		{
			return;
		}

		this->m_entities.push_back(newEntity);
	}

	void System::RemoveFromEntityList(void* data)
	{
		EntityID id = (EntityID)data;

		// Don't remove if it doesn't exist (idk how we got here)
		if (!IsEntityInList(id))
		{
			return;
		}

		// This function is also called from RemoveComponent so we ensure that we're not removing the entity itself if it is still valid.
		if (IsValidEntity(id))
		{
			return;
		}

		// Remove invalid entity
		RemoveEntityFromList(id);
	}

	void System::RemoveFromEntityListWhenDestroyed(void* data)
	{
		EntityID id = (EntityID)data;

		// Don't remove if it doesn't exist (idk how we got here)
		if (!IsEntityInList(id))
		{
			return;
		}

		// Remove invalid entity
		RemoveEntityFromList(id);
	}

	bool System::IsValidEntity(EntityID entityID)
	{
		Signature entitySignature = EntityComponentSystem::Instance()->GetSignature(entityID);
		bool validEntity = true;
		for (int i = 0; i < entitySignature.size(); ++i)
		{
			if (m_whitelist[i] == true && entitySignature[i] == false)
			{
				validEntity = false;
				break;
			}
			if (m_blacklist[i] == true && entitySignature[i] == true)
			{
				validEntity = false;
				break;
			}
		}

		return validEntity;
	}

	bool System::IsEntityInList(EntityID entityID)
	{
		for (auto* entity : m_entities)
		{
			if (entity->GetEntityID() == entityID)
			{
				return true;
			}
		}
		return false;
	}

	void System::RemoveEntityFromList(EntityID entityID)
	{
		for (size_t i = 0; i < this->m_entities.size(); i++)
		{
			if (this->m_entities[i]->GetEntityID() == entityID)
			{
				this->m_entities.erase(this->m_entities.begin() + i);
				return;
			}
		}
	}

	void System::IncrementSignatureLists()
	{
		m_blacklist.push_back(false);
		m_whitelist.push_back(false);
	}
}
