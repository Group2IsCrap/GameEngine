#include "Entity.h"

namespace Firelight::ECS
{
	Entity::Entity()
	{
		m_entityID = EntityComponentSystem::Instance()->CreateEntity();
	}

	Entity::~Entity()
	{
		EntityComponentSystem::Instance()->RemoveEntity(m_entityID);
	}
	EntityID Entity::GetEntityID()
	{
		return m_entityID;
	}
	Signature Entity::GetSignature()
	{
		return EntityComponentSystem::Instance()->GetSignature(m_entityID);
	}
}