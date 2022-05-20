#include "RockEntity.h"
#include "../Core/Layers.h"
#include <Source/Graphics/AssetManager.h>

using namespace Firelight::ECS;

RockEntity::RockEntity() : ResourceEntity()
{
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Items);
	GetLayerComponent()->layer = static_cast<int>(GameLayer::Resource);
}

RockEntity::RockEntity(Firelight::ECS::EntityID entityID) : ResourceEntity(entityID)
{
}

RockEntity::RockEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : ResourceEntity(isTemplate, entityID)
{
	GetSpriteComponent()->pixelsPerUnit = 35;
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ObjectSprites/Rock.png");
	GetComponent<TransformComponent>()->position = { -5.0f, 5.0f, 0.0f };

	GetComponent<HealthComponent>()->currentHealth = 4;

	BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider->drawCollider = true;
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 5.5f, 3.0f);
}