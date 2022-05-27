#pragma once

#include "../EntityWrappers/Entity.h"

#include "../../Utils/Time.h"

#include <iostream>

namespace Firelight::ECS
{
	class System
	{
	public:
		System();
		virtual ~System();

		std::vector<Entity*> GetEntities();

		virtual void FixedUpdate(const Utils::Time& time, const bool& isPaused);
		virtual void Update(const Utils::Time& time, const bool& isPaused);
		virtual void LateUpdate(const Utils::Time& time, const bool& isPaused);

		template<typename T>
		void AddWhitelistComponent()
		{
			ComponentTypeID typeID = EntityComponentSystem::Instance()->GetComponentType<T>();
			m_whitelist[typeID] = true;
		}
		
		template<typename T>
		void AddBlacklistComponent()
		{
			ComponentTypeID typeID = EntityComponentSystem::Instance()->GetComponentType<T>();
			m_blacklist[typeID] = true;
		}

		template<typename T>
		void RemoveWhitelistComponent()
		{
			ComponentTypeID typeID = EntityComponentSystem::Instance()->GetComponentType<T>();
			m_whitelist[typeID] = false;
		}
		
		template<typename T>
		void RemoveBlacklistComponent()
		{
			ComponentTypeID typeID = EntityComponentSystem::Instance()->GetComponentType<T>();
			m_blacklist[typeID] = false;
		}

		void AddToEntityList(void* data);
		void RemoveFromEntityList(void* data);
		void RemoveFromEntityListWhenDestroyed(void* data);
		bool IsValidEntity(EntityID entityID);
		bool IsEntityInList(EntityID entityID);
		void RemoveEntityFromList(EntityID entityID);

		void IncrementSignatureLists();

	protected:
		std::vector<Entity*> m_entities;
		Signature             m_blacklist;
		Signature             m_whitelist;

		size_t m_onEntityCreatedIndex;
		size_t m_onEntityDestroyedIndex;
		size_t m_onComponentAddedIndex;
		size_t m_onComponentRemovedIndex;
		size_t m_onComponentRegisteredIndex;
	};
}
