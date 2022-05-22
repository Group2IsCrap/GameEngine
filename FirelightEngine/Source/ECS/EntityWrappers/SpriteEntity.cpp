#include "SpriteEntity.h"

namespace Firelight::ECS
{
	SpriteEntity::SpriteEntity() : GameEntity()
	{
		AddComponent<SpriteComponent>();
		this->GetIDComponent()->name = "Sprite";
	}


	SpriteEntity::SpriteEntity(std::string name) : SpriteEntity()
	{
		this->GetIDComponent()->name = name;
	}

	SpriteComponent* SpriteEntity::GetSpriteComponent()
	{
		return GetComponent<SpriteComponent>();
	}

	SpriteEntity::SpriteEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : GameEntity(isTemplate, entityID)
	{

	}
	
}
