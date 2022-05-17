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
	AIAttackingBehaviour(RigidBodyComponent* rigidbodyComponent, AIComponent* targetAIComponent, float speed, float attackInterval);

	void HandleState(const Firelight::Utils::Time& time) override;


private:

	float m_speed;
	RigidBodyComponent* m_rigidBodyComponent;
	AIComponent* m_targetAIComponent;
};
