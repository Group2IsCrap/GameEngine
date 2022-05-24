#pragma once

#include <Source/Utils/Time.h>
#include <Source/Maths/Vec2.h>
#include <Source/Maths/Vec3.h>

#include "../AIStateBehaviour.h"
#include "../Game/Source/Core/AIEntity.h"

#include <string>
#include <random>
#include <vector>

using namespace Firelight::Maths;

/// <summary>
/// Base class for AI transition behaviour
/// </summary>
class AIAttackingBehaviour : public AIStateBehaviour
{
public:
	AIAttackingBehaviour(EntityID id, RigidBodyComponent* rigidbodyComponent, AIComponent* targetAIComponent, std::string attackAnimation, std::string walkAnimation, int damage, float speed, float attackRange, float attackCooldown, float attackRadius);

	void HandleState(AIEntity* entity, const Firelight::Utils::Time& time) override;

private:
	void Attack(Firelight::ECS::Entity* entity, Firelight::Maths::Vec3f attackPoint);

private:

	int m_damage;
	float m_attackRange;
	float m_attackTimer;
	float m_attackCooldown;
	float m_attackRadius;
	std::string m_attackAnimation;
	std::string m_walkAnimation;
	bool m_isAttacking;


	float m_speed;
	EntityID m_entityID;
	RigidBodyComponent* m_rigidBodyComponent;
	AIComponent* m_targetAIComponent;
};
