#include "SpriteEntity.h"

namespace Firelight::ECS
{
	SpriteEntity::SpriteEntity() : GameEntity()
	{
		AddComponent<SpriteComponent>();
	}

	SpriteComponent* SpriteEntity::GetSpriteComponent()
	{
		return GetComponent<SpriteComponent>();
	}

	SpriteEntity::SpriteEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : GameEntity(isTemplate, entityID)
	{

	}
	
}
