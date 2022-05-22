#include "TreeEntity.h"
#include "../Core/Layers.h"
#include <Source/Graphics/AssetManager.h>

using namespace Firelight::ECS;

TreeEntity::TreeEntity() : ResourceEntity()
{
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Items);
	GetLayerComponent()->layer = static_cast<int>(GameLayer::Resource);
}

TreeEntity::TreeEntity(Firelight::ECS::EntityID entityID) : ResourceEntity(entityID)
{
}

TreeEntity::TreeEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : ResourceEntity(isTemplate, entityID)
{
	GetSpriteComponent()->pixelsPerUnit = 35;
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ObjectSprites/Tree.png");
	GetComponent<TransformComponent>()->SetPosition({ 5.0f, 5.0f, 0.0f });
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Foreground);

	GetComponent<HealthComponent>()->currentHealth = 3;

	BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, -3.5f, 2.0f, 2.0f);
}