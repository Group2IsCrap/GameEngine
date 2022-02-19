#pragma once

#include <Source/ECS/EntityWrappers/SpriteEntity.h>

#include "../Components/HealthComponent.h"

class CharacterEntity : public Firelight::ECS::SpriteEntity
{
public:

	CharacterEntity();

public:
	HealthComponent* m_HealthComponent;
};

