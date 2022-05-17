#pragma once

#include <Source/ECS/EntityWrappers/SpriteEntity.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include <Source/Graphics/AssetManager.h>

class WorldEntity : public Firelight::ECS::SpriteEntity
{
public:
	WorldEntity();
};
