#include "AITransitionBehaviour.h"

#include "../AIBehaviourComponent.h"

#include <Source/Physics/PhysicsHelpers.h>
#include <Source/Engine.h>
#include "../../Core/Layers.h"

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
	m_wanderTimer += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime());

	if (m_wanderTimer >= 2.0f)
	{
		m_wanderTimer = 0.0f;
		
		std::vector<Firelight::ECS::Entity*> targets = Firelight::Physics::PhysicsHelpers::OverlapCircle(m_rigidBodyComponent->nextPos, searchRadius, 
			std::vector<int>
			{
				static_cast<int>(GameLayer::Player),
				static_cast<int>(GameLayer::Enemy)
			});
		for (auto* target : targets)
		{
			auto it = std::find(enemiesToTarget.begin(), enemiesToTarget.end(), target->GetComponent<IdentificationComponent>()->name);
			if (it != enemiesToTarget.end())
			{
				m_AIComponent->m_Target = target;
				m_wanderTimer = 0.0f;
				return true;
			}
		}
	}
	return false;
}

bool AITransitionBehaviour::WanderToFlee(float fleeingSearchRadius, std::vector<std::string> targetsToFleeFrom)
{
	m_fleeTimer += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime());

	if (m_fleeTimer >= 1.0f)
	{
		m_fleeTimer = 0.0f;

		std::vector<Firelight::ECS::Entity*> targets = Firelight::Physics::PhysicsHelpers::OverlapCircle(m_rigidBodyComponent->nextPos, fleeingSearchRadius,
			std::vector<int>
			{
				static_cast<int>(GameLayer::Player),
				static_cast<int>(GameLayer::Enemy)
			});
		for (auto* target : targets)
		{
			auto it = std::find(targetsToFleeFrom.begin(), targetsToFleeFrom.end(), target->GetComponent<IdentificationComponent>()->name);
			if (it != targetsToFleeFrom.end())
			{
				m_AIComponent->m_Target = target;
				m_fleeTimer = 0.0f;
				return true;
			}
		}
	}
	return false;
}

bool AITransitionBehaviour::AttackToWander(RigidBodyComponent* rigidbodyComponent, AIComponent* aiComponent, float aggroRadius)
{
	if (aiComponent->m_Target == nullptr || aiComponent->m_Target->GetComponent<RigidBodyComponent>() == nullptr || aiComponent->m_Target->GetComponent<AIComponent>() == nullptr || aiComponent->m_Target->GetComponent<AIComponent>()->isDead)
		return true;

	return Firelight::Maths::Vec3f::Dist(rigidbodyComponent->nextPos, aiComponent->m_Target->GetComponent<RigidBodyComponent>()->nextPos) >= aggroRadius;
}

bool AITransitionBehaviour::FleeToWander(float fleeingSearchRadius, std::vector<std::string> targetsToFleeFrom)
{
	m_fleeToWanderTimer += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime());

	if (m_fleeToWanderTimer >= 2.0f)
	{
		m_fleeToWanderTimer = 0.0f;

		std::vector<Firelight::ECS::Entity*> targets = Firelight::Physics::PhysicsHelpers::OverlapCircle(m_rigidBodyComponent->nextPos, fleeingSearchRadius,
			std::vector<int>
			{
				static_cast<int>(GameLayer::Player),
				static_cast<int>(GameLayer::Enemy)
			});
		for (auto* target : targets)
		{
			auto it = std::find(targetsToFleeFrom.begin(), targetsToFleeFrom.end(), target->GetComponent<IdentificationComponent>()->name);
			if (it != targetsToFleeFrom.end())
			{
				return false;
			}
		}

		m_AIComponent->m_Target = nullptr;
		m_fleeToWanderTimer = 0.0f;
		return true;
	}

	return false;
}
