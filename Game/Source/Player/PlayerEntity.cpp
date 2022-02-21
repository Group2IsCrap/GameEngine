#include "PlayerEntity.h"

#include <Source/Graphics/AssetManager.h>

#include "PlayerComponent.h"

PlayerEntity::PlayerEntity()
{
	GetComponent<Firelight::ECS::SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PlayerIdle.png");
	AddComponent<PlayerComponent>();
}
