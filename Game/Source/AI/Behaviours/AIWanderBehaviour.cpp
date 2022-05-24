#include "AIWanderBehaviour.h"

#include <Source/Maths/Random.h>
#include <Source/Maths/Vec2.h>

using namespace Firelight::Maths;

AIWanderBehaviour::AIWanderBehaviour(RigidBodyComponent* rigidbodyComponent, float wanderInterval, float speed, std::vector<std::pair<float, float>> radii) : 
	m_rigidBodyComponent(rigidbodyComponent), m_wanderInterval(wanderInterval), m_radii(radii), m_speed(speed)
{
	m_curWanderTime = 0.0f;
	m_spawnpoint = rigidbodyComponent->nextPos;
}

void AIWanderBehaviour::HandleState(const Firelight::Utils::Time& time)
{
	if (m_radii.empty())
		return;

	m_curWanderTime += time.GetDeltaTime();

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
	m_rigidBodyComponent->velocity += m_wanderLocation * m_speed * time.GetDeltaTime();
}