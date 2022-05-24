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
class AIWanderBehaviour : public AIStateBehaviour
{
public:
	AIWanderBehaviour(RigidBodyComponent* rigidbodyComponent, float wanderInterval, float speed, std::vector<std::pair<float, float>> radii);

	void HandleState(AIEntity* entity, const Firelight::Utils::Time& time) override;

	
private:
	float m_curWanderTime;

	float m_wanderInterval;
	float m_speed;
	Vec3f m_spawnpoint;

	std::vector<std::pair<float, float>> m_radii;
	Firelight::Maths::Vec3f m_wanderLocation;
	RigidBodyComponent* m_rigidBodyComponent;
};
