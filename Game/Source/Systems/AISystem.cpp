#include "AISystem.h"

#include "../Core/AIEntity.h"
#include "../AI/AIBehaviourComponent.h"

using namespace Firelight::ECS;

AISystem::AISystem()
{
	AddWhitelistComponent<AIComponent>();
	AddWhitelistComponent<AIBehaviourComponent>();
}

AISystem::~AISystem()
{

}

void AISystem::Update(const Firelight::Utils::Time& time)
{
	for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
	{
		// Get the AI entity for easy access to wrappers
		AIEntity* currentEntity = new AIEntity(m_entities[entityIndex]->GetEntityID());
		currentEntity->GetComponent<AIBehaviourComponent>()->m_CurrentTransitions->HandleTransition(time);
		currentEntity->GetComponent<AIBehaviourComponent>()->m_CurrentState->HandleState(time);

		AIComponent* aiComponent = currentEntity->GetAIComponent();
		if (aiComponent->m_tint)
		{
			aiComponent->m_tintTimer += time.GetDeltaTime();
			if (aiComponent->m_tintTimer >= aiComponent->m_tintTime)
			{
				aiComponent->m_tintTimer = 0.0f;
				aiComponent->m_tint = false;
				currentEntity->GetSpriteComponent()->colour = Firelight::Graphics::Colour::RGBA(255, 255, 255, 255);
			}
		}
	}
}
