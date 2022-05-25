#include "WorldEntity.h"
#include "Layers.h"

WorldEntity::WorldEntity() : Firelight::ECS::SpriteEntity()
{
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Terrain/water.png");
	GetSpriteComponent()->pixelsPerUnit = 20.0f;
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::World);
	this->GetIDComponent()->name = "World Entity";
}
