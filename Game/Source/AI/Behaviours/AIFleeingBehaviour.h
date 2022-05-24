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
class AIFleeingBehaviour : public AIStateBehaviour
{
public:
	AIFleeingBehaviour(EntityID id, RigidBodyComponent* rigidbodyComponent, AIComponent* targetAIComponent, std::string walkAnimation, float speed);

	void HandleState(AIEntity* entity, const Firelight::Utils::Time& time) override;

private:

	float m_speed;
	std::string m_walkAnimation;
	EntityID m_entityID;
	RigidBodyComponent* m_rigidBodyComponent;
	AIComponent* m_targetAIComponent;
};
