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
	if (/*Attacked*/nullptr)
	{
		//Flee state
	}
	else if (/*Hasn't been attacked for 10 seconds or certain distance from player*/nullptr)
	{
		//Idle state
	}
}

void AISystem::NeutralStateChanges(Entity* currentEntity)
{
	if (/*Attacked*/nullptr)
	{
		//Attack state
	}
	else if (/*Certain distance from player*/nullptr)
	{
		//Idle state
	}

}

void AISystem::HostileStateChanges(Entity* currentEntity)
{
	if (/*Attacked*/nullptr)
	{
		//Attack state
	}
	else if (/*Certain distance to player*/nullptr)
	{
		//Attack state
	}
	else if (/*Certain distance from player*/nullptr)
	{
		//Idle state
	}
}

void AISystem::Idle(Entity* currentEntity)
{
	//Wander
}

void AISystem::Attack(Entity* currentEntity)
{
	//Attack player
}

void AISystem::Flee(Entity* currentEntity)
{
	//Run away
}

