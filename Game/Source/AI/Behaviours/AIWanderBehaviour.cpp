#include "AIWanderBehaviour.h"

#include <Source/Maths/Random.h>
#include <Source/Maths/Vec2.h>
#include "../../PCG/BiomeGeneration.h"

using namespace Firelight::Maths;

AIWanderBehaviour::AIWanderBehaviour(RigidBodyComponent* rigidbodyComponent, float wanderInterval, float speed, std::vector<std::pair<float, float>> radii) : 
	m_rigidBodyComponent(rigidbodyComponent), m_wanderInterval(wanderInterval), m_radii(radii), m_speed(speed)
{
	m_curWanderTime = 0.0f;
	m_spawnpoint = rigidbodyComponent->nextPos;
}

void AIWanderBehaviour::HandleState(AIEntity* entity, const Firelight::Utils::Time& time)
{
	if (m_radii.empty())
		return;

	m_curWanderTime += static_cast<float>(time.GetDeltaTime());

	if (m_curWanderTime > m_wanderInterval)
	{
		m_curWanderTime = 0.0f;
		// Timer reached, find new random spot and set it as the point to walk towards

		float percentage = Random::ZeroToOne<float>();

		for (auto& radius : m_radii)
		{
			if (percentage < radius.first)
			{
				m_wanderLocation = Random::RandomPointInCircle<Vec3f>(m_spawnpoint, radius.second);
				break;
			}
			continue;
		}

		// Conver to direction.
		m_wanderLocation.Normalise();
	}

	// Walk towards point here
	m_rigidBodyComponent->velocity += m_wanderLocation * m_speed * static_cast<float>(time.GetDeltaTime());

	Firelight::Maths::Vec3f dir = m_wanderLocation - m_rigidBodyComponent->nextPos;
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

	Vec2f tilePos = Vec2f(targetDir.x, targetDir.y) + Vec2f(m_rigidBodyComponent->nextPos.x, m_rigidBodyComponent->nextPos.y);

	if (BiomeGeneration::Instance()->IsInVoid(tilePos))
	{
		m_rigidBodyComponent->velocity = Vec3f(0.0f, 0.0f, 0.0f);
	}
}