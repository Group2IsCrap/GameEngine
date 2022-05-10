#include "AISystem.h"

using namespace Firelight::ECS;

AISystem::AISystem()
{
	AddWhitelistComponent<AIComponent>();
	AddWhitelistComponent<CircleColliderComponent>();
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
		CircleColliderComponent* circleComponent = currentEntity->GetComponent<CircleColliderComponent>();
		
		//Call state change function based on hostility
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

		// Call behaviour function based on state
		switch (aiComponent->state)
		{
		case 1: //Idle
			Idle(currentEntity);
			break;
		case 2: //Flee
			Flee(currentEntity);
			break;
		case 3: //Attack
			Attack(currentEntity);
			break;
		}

		//Passive enemies will never have a target (they don't attack)
		if (aiComponent->hostility == 1 /*Passive*/)
		{
			aiComponent->target = nullptr;
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

Entity* AISystem::GetTarget(Entity* currentEntity)
{
	AIComponent* aiComponent = currentEntity->GetComponent<AIComponent>();
	return aiComponent->target;
}

Entity* AISystem::SelectTarget(Entity* currentEntity)
{
	CircleColliderComponent* circleComponent = currentEntity->GetComponent<CircleColliderComponent>();
}