#include "UIEntity.h"

namespace Firelight::ECS
{
	Firelight::ECS::UIEntity::UIEntity()
	{
		AddComponent<PixelSpriteComponent>();
		

	}
	PixelSpriteComponent* UIEntity::GetSpriteComponent()
	{
		return GetComponent<PixelSpriteComponent>();
	}
	
}