#include "AIWanderBehaviour.h"

AIWanderBehaviour::AIWanderBehaviour(float wanderInterval, float wanderRadius, float speed) : m_WanderInterval(wanderInterval), m_WanderRadius(wanderRadius), m_Speed(speed)
{
	m_CurWanderTime = 0.0f;
}

void AIWanderBehaviour::HandleState(const Firelight::Utils::Time& time)
{
	m_CurWanderTime += time.GetDeltaTime();

	if (m_CurWanderTime > m_WanderInterval)
	{
		m_CurWanderTime = 0.0f;
		// Timer reached, find new random spot and set it as the point to walk towards
		//These should be relative to player
		m_wanderLocation.x = (rand() % m_WanderRadius);
		m_wanderLocation.y = (rand() % m_WanderRadius);
	}

	// Walk towards point here
	//These should grab respective Entity's rigid body component
	m_rigidBodyComponent.velocity.x += m_wanderLocation.x + m_Speed * time.GetPhysicsTimeStep();
	m_rigidBodyComponent.velocity.y += m_wanderLocation.y + m_Speed * time.GetPhysicsTimeStep();
}