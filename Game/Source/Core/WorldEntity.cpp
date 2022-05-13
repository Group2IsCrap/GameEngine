#include "WorldEntity.h"
#include "Layers.h"

WorldEntity::WorldEntity()
{
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/grassTexture.png");
	GetSpriteComponent()->pixelsPerUnit = 20.0f;
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::World);
}
