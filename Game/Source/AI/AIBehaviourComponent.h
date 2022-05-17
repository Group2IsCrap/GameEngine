#pragma once
#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

#include "AIStateBehaviour.h"
#include "Transitions/AITransitionBehaviour.h"

using namespace Firelight::Serialisation;
using namespace Firelight::ECS;

struct AIBehaviourComponent : Firelight::ECS::BaseComponent
{
public:

	AITransitionBehaviour* m_CurrentTransitions = new AITransitionBehaviour(this, nullptr, nullptr, AIState::None);
	AIStateBehaviour* m_CurrentState = new AIStateBehaviour();

public:

	AIBehaviourComponent* Clone() override
	{
		AIBehaviourComponent* clone = new AIBehaviourComponent();

		return clone;
	}

};