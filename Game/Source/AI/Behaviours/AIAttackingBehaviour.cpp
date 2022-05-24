#include "AIAttackingBehaviour.h"

#include <Source/Physics/PhysicsHelpers.h>
#include "../../Core/Layers.h"
#include "../../Player/PlayerEntity.h"
#include <Source/ECS/Systems/AnimationSystem.h>

AIAttackingBehaviour::AIAttackingBehaviour(EntityID id, RigidBodyComponent* rigidbodyComponent, AIComponent* targetAIComponent, std::string animation, std::string walkAnimation, int damage, float speed, float attackRange, float attackCooldown, float attackRadius) :
	m_entityID(id),
	m_rigidBodyComponent(rigidbodyComponent), 
	m_targetAIComponent(targetAIComponent), 
	m_attackAnimation(animation),
	m_walkAnimation(walkAnimation),
	m_damage(damage),
	m_speed(speed), m_attackCooldown(attackCooldown), 
	m_attackRange(attackRange),
	m_attackRadius(attackRadius)
{
	m_attackTimer = 0.0f;
}

void AIAttackingBehaviour::HandleState(AIEntity* entity, const Firelight::Utils::Time& time)
{
	if (m_targetAIComponent == nullptr || m_targetAIComponent->m_Target == nullptr)
		return;

	RigidBodyComponent* targetRigidbody = m_targetAIComponent->m_Target->GetComponent<RigidBodyComponent>();
	Firelight::Maths::Vec3f dir = targetRigidbody->nextPos - m_rigidBodyComponent->nextPos;
	dir.Normalise();

	float colliderWidth = 0.0f;
	float colliderHeight = 0.0f;
	if (entity->HasComponent<ColliderComponent, CircleColliderComponent>())
	{
		colliderWidth = entity->GetComponent<ColliderComponent, CircleColliderComponent>()->radius / 2;
		colliderHeight = colliderWidth;
	}
	else if (entity->HasComponent<ColliderComponent, BoxColliderComponent>())
	{
		BoxColliderComponent* boxCollider = entity->GetComponent<ColliderComponent, BoxColliderComponent>();
		colliderWidth = boxCollider->rect.w / 2;
		colliderHeight = boxCollider->rect.h / 2;
	}

	Vec3f targetDir = Vec3f(colliderWidth * dir.x, colliderHeight * dir.y, 0.0f);

	// Move towards target if too far
	if ((Firelight::Maths::Vec3f::Dist(m_rigidBodyComponent->nextPos + targetDir, targetRigidbody->nextPos)) <= m_attackRange || m_isAttacking)
	{
		// Increase timer, attack if cooldown is reached
		m_attackTimer += static_cast<float>(time.GetDeltaTime());
		if (m_attackTimer >= m_attackCooldown)
		{
			m_attackTimer = 0.0f;
			Attack(entity, m_rigidBodyComponent->nextPos + targetDir);
		}
	}
	else
	{
		// Out of range, move to target
		m_rigidBodyComponent->velocity += dir * m_speed * static_cast<float>(time.GetDeltaTime());
		float magnitude = m_rigidBodyComponent->velocity.Length();
		if (m_walkAnimation != "" && magnitude > 0.05f)
		{
			AnimationSystem::Instance()->Play(entity, m_walkAnimation);
		}
	}
}

void AIAttackingBehaviour::Attack(Firelight::ECS::Entity* entity, Firelight::Maths::Vec3f attackPoint)
{
	if (m_attackAnimation != "")
	{
		AnimationSystem::Instance()->Play(entity, m_attackAnimation);
	}

	

	/*entity->GetComponent<AudioComponent>()->soundName = "Crocodile Attack.wav";
	entity->GetComponent<AudioComponent>()->soundPos = Vector3D(entity->GetComponent<TransformComponent>()->GetPosition().x, entity->GetComponent<TransformComponent>()->GetPosition().y, entity->GetComponent<TransformComponent>()->GetPosition().z);

	entity->PlayAudioClip();*/

	m_isAttacking = true;
	std::vector<Firelight::ECS::Entity*> targets = Firelight::Physics::PhysicsHelpers::OverlapCircle(attackPoint, m_attackRadius,
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

	m_isAttacking = false;
}
