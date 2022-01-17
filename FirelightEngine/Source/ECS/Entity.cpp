#include "Entity.h"

namespace Firelight::ECS
{
	Entity::Entity()
	{
		this->entityID = EntityComponentSystem::Instance()->CreateEntity();
	}

	Entity::~Entity()
	{
		EntityComponentSystem::Instance()->RemoveEntity(this->entityID);
	}
	EntityID Entity::GetEntityID()
	{
		return this->entityID;
	}
	Signature Entity::GetSignature()
	{
		return EntityComponentSystem::Instance()->GetSignature(this->entityID);
	}
}