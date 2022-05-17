#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>
#include <Source/ECS/EntityWrappers/Entity.h>

using namespace Firelight::Serialisation;
using namespace Firelight::ECS;

enum class AIType
{
	Passive,
	Neutral,
	Aggresive
};

enum class AIState
{
	None,
	Idle,
	Wandering,
	Fleeing,
	Attacking
};

struct AIComponent : Firelight::ECS::BaseComponent
{
public:

	AIType m_Type;
	Entity* m_Target;

public:

	AIComponent* Clone() override
	{
		AIComponent* clone = new AIComponent();
		clone->m_Type = m_Type;
		clone->m_Target = m_Target;

		return clone;
	}
	
};