#pragma once

#include <Source/ECS/EntityWrappers/SpriteEntity.h>

#include "../UI/HealthUI.h"

class PlayerEntity : public Firelight::ECS::SpriteEntity
{
public:
	PlayerEntity(float speed);
};