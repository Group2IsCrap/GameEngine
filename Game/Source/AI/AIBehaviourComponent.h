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

	void RenderDebugUI() override
	{
		if (m_CurrentTransitions == nullptr)
			ImGuiVariable("CurrentTransitions", 0);
		else
		{
			ImGuiVariable("State", static_cast<int>(m_CurrentTransitions->m_AIState));
		}

		if (m_CurrentState == nullptr)
			ImGuiVariable("CurrentState", 0);
		else
		{
			ImGuiVariable("CurrentState", 1);
		}
	}

public:

	AIBehaviourComponent* Clone() override
	{
		AIBehaviourComponent* clone = new AIBehaviourComponent();

		return clone;
	}

};