#include "AITransitionBehaviour.h"

#include "../AIBehaviourComponent.h"

AITransitionBehaviour::AITransitionBehaviour(AIBehaviourComponent* aiBehaviourComponent, AIState state) : m_AIComponent(aiBehaviourComponent), m_AIState(state)
{
	
}

void AITransitionBehaviour::HandleTransition(const Firelight::Utils::Time& time)
{
	if (m_Transitions.empty() || !m_Transitions.contains(m_AIState))
	{
		return;
	}

	for (std::pair<AIState, std::function<bool()>> stateFunctionPair : m_Transitions[m_AIState])
	{
		// Invoke the function
		if (stateFunctionPair.second())
		{
			// Transition
			SetState(stateFunctionPair.first);
		}
	}
}

void AITransitionBehaviour::SetState(AIState state)
{
	m_AIState = state;
	
	if (m_StateBehaviours.contains(state))
	{
		m_AIComponent->m_CurrentState = m_StateBehaviours[state];
	}
}

bool AITransitionBehaviour::IdleToWander()
{
	return true;
}
