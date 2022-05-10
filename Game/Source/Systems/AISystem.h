#pragma once
#include <Source/ECS/Systems/System.h>
#include <Source/ECS/EntityWrappers/GameEntity.h>
#include <Source/ECS/Components/PhysicsComponents.h>

#include "../CoreComponents/AIComponent.h"

using namespace Firelight::ECS;

class AISystem : public Firelight::ECS::System
{
public:
	AISystem();
	~AISystem();

	void Update(const Firelight::Utils::Time& time) override;
	void PassiveStateChanges(Entity* currentEntity, AIComponent* aiComponent);
	void NeutralStateChanges(Entity* currentEntity, AIComponent* aiComponent);
	void HostileStateChanges(Entity* currentEntity, AIComponent* aiComponent);
	void Idle(Entity* currentEntity, AIComponent* aiComponent);
	void Flee(Entity* currentEntity, AIComponent* aiComponent);
	void Attack(Entity* currentEntity, AIComponent* aiComponent);
	void SelectTarget(Entity* currentEntity, AIComponent* aiComponent, CircleColliderComponent* circleComponent);

	Entity* GetTarget(Entity* currentEntity, AIComponent* aiComponent);
};

