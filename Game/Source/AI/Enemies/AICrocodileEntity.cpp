#include "AICrocodileEntity.h"

#include <Source/Graphics/AssetManager.h>

AICrocodileEntity::AICrocodileEntity() : AIEntity()
{
	GetComponent<TransformComponent>()->position = { 10.0f, 0.0f, 0.0f };
	GetComponent<SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/ShitCroc.png");
	
}

AICrocodileEntity::AICrocodileEntity(Firelight::ECS::EntityID entityID) : AIEntity(entityID)
{
}

AICrocodileEntity::AICrocodileEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : AIEntity(isTemplate, entityID)
{
	GetComponent<TransformComponent>()->position = { 10.0f, 0.0f, 0.0f };
	GetSpriteComponent()->pixelsPerUnit = 35;
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/ShitCroc.png");

	BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider->drawCollider = true;
	boxCollider->rect = Firelight::Maths::Rectf(1.0f, -0.2f, 6.0f, 2.4f);

	BoxColliderComponent* boxCollider2 = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider2->drawCollider = true;
	boxCollider2->isTrigger = true;
	boxCollider2->rect = Firelight::Maths::Rectf(-2.3f, -0.6f, 3.5f, 0.75f);
}
