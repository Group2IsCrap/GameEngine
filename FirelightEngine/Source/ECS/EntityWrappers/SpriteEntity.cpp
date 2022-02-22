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
	
}
