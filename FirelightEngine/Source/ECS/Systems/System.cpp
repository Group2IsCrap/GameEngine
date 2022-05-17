#include "System.h"

#include "../ECSEvents.h"
#include "../../Events/EventDispatcher.h"

namespace Firelight::ECS
{
	System::System()
	{
		m_onEntityCreatedIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnEntityCreatedEvent>(std::bind(&System::UpdateEntityList, this));
		m_onEntityDestroyedIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnEntityDestroyedEvent>(std::bind(&System::UpdateEntityList, this));

		m_onComponentAddedIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnComponentAddedEvent>(std::bind(&System::UpdateEntityList, this));
		m_onComponentRemovedIndex = Events::EventDispatcher::SubscribeFunction<Events::ECS::OnComponentRemovedEvent>(std::bind(&System::UpdateEntityList, this));

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
	void System::FixedUpdate(const Utils::Time& time)
	{
		(void)time;
	}

	/// <summary>
	/// Primary update
	/// </summary>
	/// <param name="dt"></param>
	void System::Update(const Utils::Time& time)
	{
		(void)time;
	}

	/// <summary>
	/// Secondary later update
	/// </summary>
	/// <param name="dt"></param>
	void System::LateUpdate(const Utils::Time& time)
	{
		(void)time;
	}

	void System::UpdateEntityList()
	{
		m_entities.clear();

		std::vector<EntityID> ids = EntityComponentSystem::Instance()->GetEntities();

		for (auto entity : ids)
		{
			Signature entitySignature = EntityComponentSystem::Instance()->GetSignature(entity);

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
			if (!validEntity)
			{
				continue;
			}
			this->m_entities.push_back(new Entity(entity));
		}
	}

	void System::IncrementSignatureLists()
	{
		m_blacklist.push_back(false);
		m_whitelist.push_back(false);
	}
}
