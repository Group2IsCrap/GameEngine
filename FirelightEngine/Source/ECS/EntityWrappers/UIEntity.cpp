#include "UIEntity.h"
namespace Firelight::ECS
{
	Firelight::ECS::UIEntity::UIEntity()
	{
		AddComponent<NDCSpriteComponent>();
		AddComponent<UIWidget>();

		GetUIWidget()->Textuer = GetSpriteComponent();
		GetUIWidget()->Transform = GetTransformComponent();

	}
	NDCSpriteComponent* UIEntity::GetSpriteComponent()
	{
		return GetComponent<NDCSpriteComponent>();
	}
	UIWidget* UIEntity::GetUIWidget()
	{
		return GetComponent<UIWidget>();
	}
}