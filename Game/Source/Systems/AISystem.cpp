#include "AISystem.h"

#include "../Core/AIEntity.h"
#include "../AI/AIBehaviourComponent.h"
#include "../PCG/BiomeGeneration.h"

using namespace Firelight::ECS;

AISystem::AISystem()
{
	AddWhitelistComponent<AIComponent>();
	AddWhitelistComponent<AIBehaviourComponent>();
}

AISystem::~AISystem()
{

}

void AISystem::Update(const Firelight::Utils::Time& time, const bool& isPaused)
{
	if (!isPaused)
	{
		for (int entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
		{
			// Get the AI entity for easy access to wrappers
			AIEntity currentEntity = AIEntity(m_entities[entityIndex]->GetEntityID());
			AIBehaviourComponent* aiBehaviourComponment = currentEntity.GetComponent<AIBehaviourComponent>();
			aiBehaviourComponment->m_CurrentTransitions->HandleTransition(time);
			aiBehaviourComponment->m_CurrentState->HandleState(&currentEntity, time);
			currentEntity.FaceDirection();

			AIComponent* aiComponent = currentEntity.GetAIComponent();
			if (aiComponent->m_tint)
			{
				aiComponent->m_tintTimer += static_cast<float>(time.GetDeltaTime());
				if (aiComponent->m_tintTimer >= aiComponent->m_tintTime)
				{
					aiComponent->m_tintTimer = 0.0f;
					aiComponent->m_tint = false;
					currentEntity.GetSpriteComponent()->colour = Firelight::Graphics::Colour::RGBA(255, 255, 255, 255);
				}
			}
		}
	}
}
