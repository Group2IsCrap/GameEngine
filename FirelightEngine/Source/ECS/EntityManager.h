#pragma once

#include "ECSDefines.h"

#include <vector>
#include <unordered_map>

namespace Firelight::ECS
{
	typedef std::vector<bool> Signature;

	class EntityManager
	{
	public:
		EntityID CreateEntity();
		void UpdateEntitySignature(EntityID entity, ComponentTypeID componentID, bool contains);
		void RemoveEntity(EntityID entity);
		Signature GetEntitySignature(EntityID entity);
		std::vector<EntityID> GetEntities();

		EntityManager() = default;
		~EntityManager() = default;
	private:
		std::vector<EntityID> m_entities;
		std::unordered_map<EntityID,Signature> m_signatures;

		static EntityID sm_nextEntity;

	};
}
