#pragma once

#include <Source/Utils/Time.h>
#include <Source/Maths/Vec2.h>

#include "../AIStateBehaviour.h"
#include "../Game/Source/Core/AIEntity.h"

#include <random>


/// <summary>
/// Base class for AI transition behaviour
/// </summary>
class AIWanderBehaviour : public AIStateBehaviour
{
public:
	AIWanderBehaviour(float wanderInterval, float wanderRadius, float speed);

	void HandleState(const Firelight::Utils::Time& time) override;

	
private:
	float m_CurWanderTime;

	float m_WanderInterval;
	float m_Speed;

	int m_WanderRadius;

	Firelight::Maths::Vec2f m_wanderLocation;
	RigidBodyComponent m_rigidBodyComponent;
};
