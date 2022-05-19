#pragma once

#include <Source/Utils/Time.h>
#include <Source/Maths/Vec2.h>
#include <Source/Maths/Vec3.h>

#include "../AIStateBehaviour.h"
#include "../Game/Source/Core/AIEntity.h"

#include <random>
#include <vector>

using namespace Firelight::Maths;

/// <summary>
/// Base class for AI transition behaviour
/// </summary>
class AIAttackingBehaviour : public AIStateBehaviour
{
public:
	AIAttackingBehaviour(EntityID id, RigidBodyComponent* rigidbodyComponent, AIComponent* targetAIComponent, int damage, float speed, float attackRange, float attackCooldown, float attackRadius);

	void HandleState(const Firelight::Utils::Time& time) override;

private:
	void Attack(Firelight::Maths::Vec3f dir);

private:

	int m_damage;
	float m_attackRange;
	float m_attackTimer;
	float m_attackCooldown;
	float m_attackRadius;

	float m_speed;
	EntityID m_entityID;
	RigidBodyComponent* m_rigidBodyComponent;
	AIComponent* m_targetAIComponent;
};
