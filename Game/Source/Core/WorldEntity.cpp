#include "WorldEntity.h"

WorldEntity::WorldEntity()
{
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
	GetSpriteComponent()->pixelsPerUnit = 20.0f;
	GetSpriteComponent()->layer = 16;
}
