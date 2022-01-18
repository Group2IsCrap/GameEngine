#include "Entity.h"

namespace Firelight::ECS
{
	Entity::Entity()
	{
		entityID = EntityComponentSystem::Instance()->CreateEntity();
	}

	Entity::Entity(EntityID id)
	{
		entityID = EntityComponentSystem::Instance()->CreateEntity(id);
	}

	Entity::~Entity()
	{
		EntityComponentSystem::Instance()->RemoveEntity(entityID);
	}
	EntityID Entity::GetEntityID()
	{
		return entityID;
	}
	Signature Entity::GetSignature()
	{
		return EntityComponentSystem::Instance()->GetSignature(entityID);
	}
}