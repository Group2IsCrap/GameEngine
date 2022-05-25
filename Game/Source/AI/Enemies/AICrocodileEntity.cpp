#include "AICrocodileEntity.h"

#include <Source/Graphics/AssetManager.h>
#include "../AIBehaviourComponent.h"
#include "../Behaviours/AIWanderBehaviour.h"
#include "../Behaviours/AIAttackingBehaviour.h"
#include "../../Core/Layers.h"

AICrocodileEntity::AICrocodileEntity() : AIEntity()
{
	GetComponent<SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/CrocIdle.png");
	
}

AICrocodileEntity::AICrocodileEntity(Firelight::ECS::EntityID entityID) : AIEntity(entityID)
{
}

AICrocodileEntity::AICrocodileEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : AIEntity(isTemplate, entityID)
{
	GetIDComponent()->name = "Crocodile";
	//GetComponent<RigidBodyComponent>()->nextPos = { 2.0f, 0.0f, 0.0f };
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/CrocIdle.png");
	GetSpriteComponent()->pixelsPerUnit = 100;
	
	GetRigidBodyComponent()->interpolate = true;

	GetComponent<HealthComponent>()->maxHealth = 5;
	GetComponent<HealthComponent>()->currentHealth = GetMaxHealth();

	BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 9.0f, 2.4f);

	AIBehaviourComponent* aiBehaviourComponent = GetComponent<AIBehaviourComponent>();

	std::vector<std::pair<float, float>> radii
	{
		std::make_pair(0.1f, 1.0f),
		std::make_pair(0.4f, 0.6f),
		std::make_pair(0.8f, 0.2f)
	};


	aiBehaviourComponent->m_CurrentTransitions = new AITransitionBehaviour(aiBehaviourComponent, GetAIComponent(), GetRigidBodyComponent(), AIState::None);
	AIStateBehaviour* wanderBehaviour = new AIWanderBehaviour(GetRigidBodyComponent(), 4.0f, 0.5f, radii);
	aiBehaviourComponent->m_CurrentTransitions->m_StateBehaviours[AIState::Wandering] = wanderBehaviour;

	AIStateBehaviour* attackingBehaviour = new AIAttackingBehaviour(GetEntityID(), GetRigidBodyComponent(), GetAIComponent(), "CrocodileIdle", "CrocodileWalk", 4, 0.8f, 2.0f, 2.0f, 1.0f, "Crocodile Attack.wav");
	aiBehaviourComponent->m_CurrentTransitions->m_StateBehaviours[AIState::Attacking] = attackingBehaviour;

	std::map<AIState, std::function<bool()>> idleTransitions;
	idleTransitions[AIState::Wandering] = std::bind(&AITransitionBehaviour::IdleToWander, aiBehaviourComponent->m_CurrentTransitions);
	aiBehaviourComponent->m_CurrentTransitions->m_Transitions[AIState::Idle] = idleTransitions;

	std::map<AIState, std::function<bool()>> wanderTransitions;
	std::vector<int> layers = { static_cast<int>(GameLayer::Enemy), static_cast<int>(GameLayer::Player) };
	std::vector<std::string> targets = { "Player", "Deer" };
	wanderTransitions[AIState::Attacking] = std::bind(&AITransitionBehaviour::WanderToAttack, aiBehaviourComponent->m_CurrentTransitions, 10.0f, layers, targets);
	aiBehaviourComponent->m_CurrentTransitions->m_Transitions[AIState::Wandering] = wanderTransitions;

	std::map<AIState, std::function<bool()>> attackingTransitions;
	attackingTransitions[AIState::Wandering] = std::bind(&AITransitionBehaviour::AttackToWander, aiBehaviourComponent->m_CurrentTransitions, GetRigidBodyComponent(), GetAIComponent(), 20.0f);
	aiBehaviourComponent->m_CurrentTransitions->m_Transitions[AIState::Attacking] = attackingTransitions;

	aiBehaviourComponent->m_CurrentTransitions->SetState(AIState::Idle);
}


