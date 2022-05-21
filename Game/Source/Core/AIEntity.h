#pragma once
#include <Source/ECS/Components/PhysicsComponents.h>

#include "../Core/CharacterEntity.h"
#include "../CoreComponents/AIComponent.h"

class AIEntity : public CharacterEntity
{
public:

	AIEntity();
	AIEntity(std::string name);
	AIEntity(Firelight::ECS::EntityID entityID);
	AIEntity(bool isTemplate, Firelight::ECS::EntityID entityID);

	void RemoveHealth(int amount) override;
	void HealthBelowZero() override;
	void FaceDirection();

	AIComponent* GetAIComponent();

	AIType GetType();

	Entity* GetTarget();

private:
	AIComponent* m_AIComponent;
	RigidBodyComponent* m_rigidbodyComponent;
	SpriteComponent* m_spriteComponent;
	TransformComponent* m_transformComponent;
};
