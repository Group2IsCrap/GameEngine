#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Events/Event.h>

#include "../Core/CharacterEntity.h"

class PlayerEntity : public CharacterEntity
{
public:
	PlayerEntity();
	PlayerEntity(Firelight::ECS::EntityID entityID);

	void HealthBelowZero() override;
	void RemoveHealth(int amount) override;
	void AddHealth(int amount) override;
	void PlayerHealthUpdated();
};