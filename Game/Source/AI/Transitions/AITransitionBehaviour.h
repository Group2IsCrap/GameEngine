#pragma once

#include <Source/Utils/Time.h>

#include "../../CoreComponents/AIComponent.h"

#include <map>

class AIBehaviourComponent;
class AIStateBehaviour;

/// <summary>
/// Base class for AI transition behaviour
/// </summary>
class AITransitionBehaviour
{
public:
	AITransitionBehaviour(AIBehaviourComponent* aiBehaviourComponent, AIState state);

	virtual void HandleTransition(const Firelight::Utils::Time& time);
	void SetState(AIState state);

	// Define new default transitions here.
	bool IdleToWander();

public:
	std::map<AIState, std::map<AIState, std::function<bool()>>> m_Transitions;
	std::map<AIState, AIStateBehaviour*> m_StateBehaviours;
	AIState m_AIState;
	AIBehaviourComponent* m_AIComponent;
};
