#pragma once

#include <Source/Utils/Time.h>

#include "../../CoreComponents/AIComponent.h"
#include <Source/ECS/Components/PhysicsComponents.h>

#include <map>
#include <vector>

struct AIBehaviourComponent;
class AIStateBehaviour;

/// <summary>
/// Base class for AI transition behaviour
/// </summary>
class AITransitionBehaviour
{
public:
	AITransitionBehaviour(AIBehaviourComponent* aiBehaviourComponent, AIComponent* aiComponent, Firelight::ECS::RigidBodyComponent* rigidbodyComponent, AIState state);

	virtual void HandleTransition(const Firelight::Utils::Time& time);
	void SetState(AIState state);

	// Define new default transitions here.
	bool IdleToWander();

	bool WanderToAttack(float searchRadius, std::vector<int> layers, std::vector<std::string> enemiesToTarget);
	bool WanderToFlee(float fleeingSearchRadius, std::vector<std::string> targetsToFleeFrom);

	bool AttackToWander(RigidBodyComponent* rigidbodyComponent, AIComponent* aiComponent, float aggroRadius);

	bool FleeToWander(float fleeingSearchRadius, std::vector<std::string> targetsToFleeFrom);

public:
	std::map<AIState, std::map<AIState, std::function<bool()>>> m_Transitions;
	std::map<AIState, AIStateBehaviour*> m_StateBehaviours;
	AIState m_AIState;
	AIBehaviourComponent* m_AIBehaviourComponent;
	AIComponent* m_AIComponent;
	Firelight::ECS::RigidBodyComponent* m_rigidBodyComponent;

private:
	float m_wanderTimer;
	float m_fleeTimer;
	float m_fleeToWanderTimer;
};
