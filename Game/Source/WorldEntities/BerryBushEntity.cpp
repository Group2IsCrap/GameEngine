#include "BerryBushEntity.h"
#include "../Core/Layers.h"
#include <Source/Graphics/AssetManager.h>

using namespace Firelight::ECS;

BerryBushEntity::BerryBushEntity() : ResourceEntity()
{
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Items);
	GetLayerComponent()->layer = static_cast<int>(GameLayer::Resource);
}

BerryBushEntity::BerryBushEntity(Firelight::ECS::EntityID entityID) : ResourceEntity(entityID)
{
}

BerryBushEntity::BerryBushEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : ResourceEntity(isTemplate, entityID)
{
	GetSpriteComponent()->pixelsPerUnit = 35;
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ObjectSprites/BerryBush.png");
	GetComponent<TransformComponent>()->SetPosition({ -5.0f, -5.0f, 0.0f });

	GetComponent<HealthComponent>()->currentHealth = 2;

	BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider->isTrigger = true;
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, -0.2f, 3.0f, 1.0f);
}