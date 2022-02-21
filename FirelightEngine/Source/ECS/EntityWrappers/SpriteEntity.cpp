#include "SpriteEntity.h"

namespace Firelight::ECS
{
	SpriteEntity::SpriteEntity() :
		GameEntity()
	{
		AddComponent<SpriteComponent>();
		
	}

	SpriteEntity::SpriteEntity(Firelight::ECS::EntityID entityID) : GameEntity(entityID)
	{

	}

	SpriteComponent* SpriteEntity::GetSpriteComponent()
	{
		return GetComponent<SpriteComponent>();
	}
	
}
