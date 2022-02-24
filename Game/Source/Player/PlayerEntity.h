#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Events/Event.h>

#include "../Core/CharacterEntity.h"

namespace Firelight::Events
{
	NEW_EVENT(PlayerHealthChanged);
	NEW_EVENT(PlayerDied);
}

class PlayerEntity : public CharacterEntity
{
public:
	PlayerEntity();
	PlayerEntity(Firelight::ECS::EntityID entityID);

	void HealthBelowZero() override;
	void RemoveHealth(int amount) override;
	void PlayerHealthUpdated();
};

