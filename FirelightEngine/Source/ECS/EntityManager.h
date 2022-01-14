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
		Entity CreateEntity();
		void UpdateEntitySignature(Entity entity, ComponentTypeID componentID, bool contains);
		void RemoveEntity(Entity entity);
		Signature GetEntitySignature(Entity entity);
		std::vector<Entity> GetEntities();
		//void DebugEntities();

		EntityManager() = default;
		~EntityManager() = default;
	private:
		std::vector<Entity> m_entities;
		std::unordered_map<Entity,Signature> m_signatures;

		static Entity sm_nextEntity;

	};
}
