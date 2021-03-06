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
	AIType m_Type;
	Entity* m_Target;
	bool isDead;

	float m_tintTime = 0.2f;
	float m_tintTimer;
	bool m_tint = false;

	AIComponent* Clone() override
	{
		AIComponent* clone = new AIComponent();
		clone->m_Type = m_Type;
		clone->m_Target = m_Target;

		return clone;
	}

	void RenderDebugUI() override
	{
		ImGuiVariable("Type", (int)m_Type);
		if (m_Target != nullptr)
			ImGuiVariable("Target", (int)m_Target->GetEntityID());
		else 
			ImGuiVariable("Target", 0);

		ImGuiVariable("TintTime", m_tintTime);
		ImGuiVariable("TintTimer", m_tintTimer);
		ImGuiVariable("Tint", m_tint);
	}
};