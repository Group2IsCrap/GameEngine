#include "AISystem.h"
#include "../CoreComponents/AIComponent.h"

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
		Entity* currentEntity = m_entities[entityIndex];
		AIComponent* aiComponent = currentEntity->GetComponent<AIComponent>();
		
		switch (aiComponent->hostility)
		{
		case 1: //Passive
			PassiveStateChanges(currentEntity);
			break;
		case 2: //Neutral
			NeutralStateChanges(currentEntity);
			break;
		case 3: //Hostile
			HostileStateChanges(currentEntity);
			break;
		}
	}
}

void AISystem::PassiveStateChanges(Entity* currentEntity)
{

}

void AISystem::NeutralStateChanges(Entity* currentEntity)
{

}

void AISystem::HostileStateChanges(Entity* currentEntity)
{

}

