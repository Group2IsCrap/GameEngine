#include "AISystem.h"

#include "../Core/AIEntity.h"

using namespace Firelight::ECS;

AISystem::AISystem()
{
	AddWhitelistComponent<AIComponent>();
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
		HandleState(currentEntity);
	}
}

void AISystem::HandleState(AIEntity* aiEntity)
{
	switch (aiEntity->GetState())
	{
	case AIState::Idle:
		
		break;
	case AIState::Wandering:

		break;
	case AIState::Attacking:

		break;
	case AIState::Fleeing:

		break;
	}
}
