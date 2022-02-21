#pragma once

#include <Source/ECS/EntityWrappers/SpriteEntity.h>

#include "../CoreComponents/HealthComponent.h"

class CharacterEntity : public Firelight::ECS::SpriteEntity
{
public:

	CharacterEntity();

public:
	HealthComponent* m_HealthComponent;
};

