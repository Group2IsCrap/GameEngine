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
	void PassiveStateChanges(Entity* currentEntity);
	void NeutralStateChanges(Entity* currentEntity);
	void HostileStateChanges(Entity* currentEntity);
	void Idle(Entity* currentEntity);
	void Flee(Entity* currentEntity);
	void Attack(Entity* currentEntity);

	Entity* GetTarget(Entity* currentEntity);

	Entity* SelectTarget(Entity* currentEntity);
};

