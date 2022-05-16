#pragma once

#include <Source/Utils/Time.h>

#include "../AIStateBehaviour.h"

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
	float m_WanderRadius;
	float m_Speed;
};
