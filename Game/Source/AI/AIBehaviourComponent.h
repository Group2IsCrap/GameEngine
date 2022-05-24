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

	AITransitionBehaviour* m_CurrentTransitions = nullptr;
	AIStateBehaviour* m_CurrentState = nullptr;

public:

	AIBehaviourComponent* Clone() override
	{
		AIBehaviourComponent* clone = new AIBehaviourComponent();

		return clone;
	}

};