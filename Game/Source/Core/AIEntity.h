#pragma once
#include <Source/ECS/Components/PhysicsComponents.h>

#include "../Core/CharacterEntity.h"
#include "../CoreComponents/AIComponent.h"

class AIEntity : public CharacterEntity
{
public:

	AIEntity();
	AIEntity(Firelight::ECS::EntityID entityID);
	AIEntity(bool isTemplate, Firelight::ECS::EntityID entityID);

	void RemoveHealth(int amount) override;
	void HealthBelowZero() override;

	AIComponent* GetAIComponent();

	AIType GetType();

	Entity* GetTarget();

private:
	AIComponent* m_AIComponent;
};
