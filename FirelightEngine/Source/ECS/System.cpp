#include "System.h"

#include "ECSEvents.h"
#include "../Events/EventDispatcher.h"

namespace Firelight::ECS
{
	System::System()
	{
		Events::EventDispatcher::SubscribeFunction(Events::ECS::OnEntityCreatedEvent::sm_descriptor, std::bind(&System::UpdateEntityList, this));
		Events::EventDispatcher::SubscribeFunction(Events::ECS::OnEntityDestroyedEvent::sm_descriptor, std::bind(&System::UpdateEntityList, this));
	}

	System::~System()
	{
	}
	/// <summary>
	/// Returns a list of the maintained entities
	/// </summary>
	/// <returns></returns>
	std::vector<EntityID> System::GetEntities()
	{
		return m_entities;
	}
	/// <summary>
	/// Primary Update Loop
	/// </summary>
	/// <param name="dt"></param>
	void System::Update(double dt)
	{
		(void)dt;
	}
	/// <summary>
	/// Secondary Update Loop
	/// </summary>
	/// <param name="dt"></param>
	void System::LateUpdate(double dt)
	{
		(void)dt;
	}
	/// <summary>
	/// Fixed Update loop
	/// </summary>
	/// <param name="fixeddt"></param>
	void System::FixedUpdate(double fixeddt)
	{
		(void)fixeddt;
	}


	void System::UpdateEntityList()
	{
		std::vector<EntityID> ids = EntityComponentSystem::Instance()->GetEntities();

		for (auto entity : ids)
		{
			Signature entitySignature = EntityComponentSystem::Instance()->GetSignature(entity);

			bool validEntity = true;
			for (int i = 0; i < entitySignature.size(); ++i)
			{
				if (whitelist[i] == true && entitySignature[i] == false)
				{
					validEntity = false;
					break;
				}
			}
			if (!validEntity)
			{
				continue;
			}
			for (int i = 0; i < entitySignature.size(); ++i)
			{
				if (blacklist[i] == true && entitySignature[i] == true)
				{
					validEntity = false;
					break;
				}
			}
			if (!validEntity)
			{
				continue;
			}
			this->m_entities.push_back(entity);
		}
	}
}