#include "AIDeerEntity.h"

#include <Source/Graphics/AssetManager.h>
#include "../AIBehaviourComponent.h"
#include "../Behaviours/AIWanderBehaviour.h"

AIDeerEntity::AIDeerEntity() : AIEntity()
{
	GetComponent<TransformComponent>()->position = { 10.0f, 0.0f, 0.0f };
	GetComponent<SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/ShitDeer.png");

}

AIDeerEntity::AIDeerEntity(Firelight::ECS::EntityID entityID) : AIEntity(entityID)
{
}

AIDeerEntity::AIDeerEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : AIEntity(isTemplate, entityID)
{
	GetSpriteComponent()->pixelsPerUnit = 50;
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/ShitDeer.png");

	BoxColliderComponent* boxCollider = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider->drawCollider = true;
	boxCollider->rect = Firelight::Maths::Rectf(0.5f, -0.75f, 2.3f, 2.4f);

	BoxColliderComponent* boxCollider2 = dynamic_cast<BoxColliderComponent*>(AddComponent<ColliderComponent>(new BoxColliderComponent()));
	boxCollider2->drawCollider = true;
	boxCollider2->rect = Firelight::Maths::Rectf(-1.0f, 0.55f, 1.5f, 2.3f);

	AIBehaviourComponent* aiBehaviourComponent = GetComponent<AIBehaviourComponent>();

	AIStateBehaviour* wanderBehaviour = new AIWanderBehaviour(1.0f, 1.0f, 1.0f);
	aiBehaviourComponent->m_CurrentTransitions->m_StateBehaviours.insert(std::pair<AIState, AIStateBehaviour*>(AIState::Wandering, wanderBehaviour));

	std::map<AIState, std::function<bool()>> idleTransitions;
	idleTransitions[AIState::Wandering] = std::bind(&AITransitionBehaviour::IdleToWander, aiBehaviourComponent->m_CurrentTransitions);
	aiBehaviourComponent->m_CurrentTransitions->m_Transitions[AIState::Idle] = idleTransitions;

	aiBehaviourComponent->m_CurrentTransitions->SetState(AIState::Idle);
}
