#include "AIAttackingBehaviour.h"

#include <Source/Physics/PhysicsHelpers.h>
#include "../../Core/Layers.h"
#include "../../Player/PlayerEntity.h"

AIAttackingBehaviour::AIAttackingBehaviour(EntityID id, RigidBodyComponent* rigidbodyComponent, AIComponent* targetAIComponent, int damage, float speed, float attackRange, float attackCooldown, float attackRadius) :
	m_entityID(id),
	m_rigidBodyComponent(rigidbodyComponent), 
	m_targetAIComponent(targetAIComponent), 
	m_damage(damage),
	m_speed(speed), m_attackCooldown(attackCooldown), 
	m_attackRange(attackRange),
	m_attackRadius(attackRadius)
{
	m_attackTimer = 0.0f;
}

void AIAttackingBehaviour::HandleState(const Firelight::Utils::Time& time)
{
	if (m_targetAIComponent == nullptr || m_targetAIComponent->m_Target == nullptr)
		return;

	RigidBodyComponent* targetRigidbody = m_targetAIComponent->m_Target->GetComponent<RigidBodyComponent>();
	Firelight::Maths::Vec3f dir = targetRigidbody->nextPos - m_rigidBodyComponent->nextPos;
	dir.Normalise();

	// Move towards target if too far
	if (Firelight::Maths::Vec3f::Dist(m_rigidBodyComponent->nextPos, targetRigidbody->nextPos) <= m_attackRange)
	{
		// Increase timer, attack if cooldown is reached
		m_attackTimer += time.GetDeltaTime();
		if (m_attackTimer >= m_attackCooldown)
		{
			m_attackTimer = 0.0f;
			Attack(dir);
		}
	}
	else
	{
		// Out of range, move to target
		m_rigidBodyComponent->velocity += dir * m_speed * time.GetDeltaTime();
	}
}

void AIAttackingBehaviour::Attack(Firelight::Maths::Vec3f dir)
{
	std::vector<Firelight::ECS::Entity*> targets = Firelight::Physics::PhysicsHelpers::OverlapCircle(m_rigidBodyComponent->nextPos + dir, m_attackRadius, 
		std::vector<int>
	{
		static_cast<int>(GameLayer::Enemy),
		static_cast<int>(GameLayer::Player)
	});

	for (auto* target : targets)
	{
		if (target->GetEntityID() == m_entityID)
		{
			continue;
		}

		if (target->GetComponent<LayerComponent>()->layer == static_cast<int>(GameLayer::Player))
		{
			PlayerEntity* playerEntity = new PlayerEntity(target->GetEntityID());
			playerEntity->RemoveHealth(m_damage);
		}
		else if (target->GetComponent<LayerComponent>()->layer == static_cast<int>(GameLayer::Enemy))
		{
			AIEntity* aiEntity = new AIEntity(target->GetEntityID());
			aiEntity->RemoveHealth(m_damage);
		}
	}
}
