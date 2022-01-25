#include "UIEntity.h"
namespace Firelight::ECS
{
	Firelight::ECS::UIEntity::UIEntity()
	{
		AddComponent<PixleSpriteComponent>();
		

	}
	PixleSpriteComponent* UIEntity::GetSpriteComponent()
	{
		return GetComponent<PixleSpriteComponent>();
	}
	
}