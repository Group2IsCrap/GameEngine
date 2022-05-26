#include "AIDeerEntity.h"

#include <Source/Graphics/AssetManager.h>
#include "../AIBehaviourComponent.h"
#include "../Behaviours/AIWanderBehaviour.h"
#include "../Behaviours/AIFleeingBehaviour.h"

AIDeerEntity::AIDeerEntity() : AIEntity()
{
	GetComponent<TransformComponent>()->SetPosition({10.0f, 0.0f, 0.0f});
	GetComponent<SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/DeerIdle.png");

}

AIDeerEntity::AIDeerEntity(Firelight::ECS::EntityID entityID) : AIEntity(entityID)
{
}

AIDeerEntity::AIDeerEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : AIEntity(isTemplate, entityID)
{
	GetIDComponent()->name = "Deer";
	GetSpriteComponent()->pixelsPerUnit = 50;

	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/DeerIdle.png");

	GetRigidBodyComponent()->interpolate = true;
	GetHealthComponent()->maxHealth = 5;
	GetHealthComponent()->currentHealth = GetHealthComponent()->maxHealth;

	BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 4.0f, 4.0f);

	AIBehaviourComponent* aiBehaviourComponent = GetComponent<AIBehaviourComponent>();

	std::vector<std::pair<float, float>> radii
	{ 
		std::make_pair(0.8f, 0.2f),
		std::make_pair(0.4f, 0.6f),
		std::make_pair(0.1f, 1.0f)
	};

	aiBehaviourComponent->m_CurrentTransitions = new AITransitionBehaviour(aiBehaviourComponent, GetAIComponent(), GetRigidBodyComponent(), AIState::None);
	
	AIStateBehaviour* wanderBehaviour = new AIWanderBehaviour(GetRigidBodyComponent(), 4.0f, 0.5f, radii);
	AIStateBehaviour* fleeBehaviour = new AIFleeingBehaviour(GetEntityID(), GetRigidBodyComponent(), GetAIComponent(), "DeerWalk", 1.0f);

	aiBehaviourComponent->m_CurrentTransitions->m_StateBehaviours[AIState::Wandering] = wanderBehaviour;
	aiBehaviourComponent->m_CurrentTransitions->m_StateBehaviours[AIState::Fleeing] = fleeBehaviour;
	
	std::map<AIState, std::function<bool()>> idleTransitions;
	idleTransitions[AIState::Wandering] = std::bind(&AITransitionBehaviour::IdleToWander, aiBehaviourComponent->m_CurrentTransitions);
	aiBehaviourComponent->m_CurrentTransitions->m_Transitions[AIState::Idle] = idleTransitions;

	std::vector<std::string> targets = { "Player", "Crocodile", "Bear" };

	std::map<AIState, std::function<bool()>> wanderTransitions;
	wanderTransitions[AIState::Fleeing] = std::bind(&AITransitionBehaviour::WanderToFlee, aiBehaviourComponent->m_CurrentTransitions, 10.0f, targets);
	aiBehaviourComponent->m_CurrentTransitions->m_Transitions[AIState::Wandering] = wanderTransitions;

	std::map<AIState, std::function<bool()>> fleeingTransitions;
	fleeingTransitions[AIState::Wandering] = std::bind(&AITransitionBehaviour::FleeToWander, aiBehaviourComponent->m_CurrentTransitions, 10.0f, targets);
	aiBehaviourComponent->m_CurrentTransitions->m_Transitions[AIState::Fleeing] = fleeingTransitions;

	aiBehaviourComponent->m_CurrentTransitions->SetState(AIState::Idle);
}
