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
		EntityID CreateEntity(EntityID id);
		void UpdateEntitySignature(EntityID entity, ComponentTypeID componentID, bool contains);
		void CreateNewEntitySignature(EntityID entity, int numComponents);
		void ClearSignatures();
		void RemoveEntity(EntityID entity);
		Signature GetEntitySignature(EntityID entity);
		std::vector<EntityID> GetEntities();

		EntityManager() = default;
		~EntityManager() = default;
	private:
		EntityID CreateEntityInternal(EntityID id);
	private:
		std::vector<EntityID> m_entities;
		std::unordered_map<EntityID,Signature> m_signatures;

		static EntityID sm_nextEntity;

	};
}
