#pragma once

#include <Source/ECS/EntityWrappers/SpriteEntity.h>

#include "../CoreComponents/HealthComponent.h"

class CharacterEntity : public Firelight::ECS::SpriteEntity
{
public:

	CharacterEntity();

	HealthComponent* GetHealthComponent();
	int GetHealth();
	virtual void RemoveHealth(int amount);
	virtual void HealthBelowZero();

public:
	HealthComponent* m_HealthComponent;
};

