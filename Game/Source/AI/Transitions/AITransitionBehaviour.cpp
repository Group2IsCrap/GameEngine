#include "AITransitionBehaviour.h"

#include "../AIBehaviourComponent.h"

#include <Source/Physics/PhysicsHelpers.h>
#include <Source/Engine.h>

AITransitionBehaviour::AITransitionBehaviour(AIBehaviourComponent* aiBehaviourComponent, AIComponent* aiComponent, Firelight::ECS::RigidBodyComponent* rigidbodyComponent, AIState state) : m_AIBehaviourComponent(aiBehaviourComponent), m_AIComponent(aiComponent), m_rigidBodyComponent(rigidbodyComponent), m_AIState(state)
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
		m_AIBehaviourComponent->m_CurrentState = m_StateBehaviours[state];
	}
}

bool AITransitionBehaviour::IdleToWander()
{
	return true;
}

bool AITransitionBehaviour::WanderToAttack(float searchRadius, std::vector<int> layers, std::vector<std::string> enemiesToTarget)
{
	m_wanderTimer += Firelight::Engine::Instance().GetTime().GetDeltaTime();

	if (m_wanderTimer >= 2.0f)
	{
		m_wanderTimer = 0.0f;
		
		std::vector<Firelight::ECS::Entity*> targets = Firelight::Physics::PhysicsHelpers::OverlapCircle(m_rigidBodyComponent->nextPos, searchRadius, 0);
		for (auto* target : targets)
		{
			auto it = std::find(enemiesToTarget.begin(), enemiesToTarget.end(), target->GetComponent<IdentificationComponent>()->name);
			if (it != enemiesToTarget.end())
			{
				m_AIComponent->m_Target = target;
				return true;
			}
		}
	}
	return false;
}

bool AITransitionBehaviour::AttackToWander(RigidBodyComponent* rigidbodyComponent, AIComponent* aiComponent, float aggroRadius)
{
	return Firelight::Maths::Vec3f::Dist(rigidbodyComponent->nextPos, aiComponent->m_Target->GetComponent<RigidBodyComponent>()->nextPos) >= aggroRadius;
}
