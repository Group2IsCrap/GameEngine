#pragma once

#include "../Entity.h"

#include <iostream>

namespace Firelight::ECS
{
	class System
	{
	public:
		System();
		virtual ~System();

		std::vector<EntityID> GetEntities();
		virtual void Update(double dt);
		virtual void LateUpdate(double dt);
		virtual void FixedUpdate(double fixeddt);

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

		void UpdateEntityList();

		void IncrementSignatureLists();

	protected:
		std::vector<EntityID> m_entities;
		Signature             m_blacklist;
		Signature             m_whitelist;
	};
}
