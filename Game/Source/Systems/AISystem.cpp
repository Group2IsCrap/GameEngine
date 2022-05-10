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

		circleComponent->radius = 5.0f;
		
		//Call state change function based on hostility
		switch (aiComponent->hostility)
		{
		case 1: //Passive
			PassiveStateChanges(currentEntity, aiComponent);
			break;
		case 2: //Neutral
			NeutralStateChanges(currentEntity, aiComponent);
			break;
		case 3: //Hostile
			HostileStateChanges(currentEntity, aiComponent);
			break;
		}

		// Call behaviour function based on state
		switch (aiComponent->state)
		{
		case 1: //Idle
			Idle(currentEntity, aiComponent);
			break;
		case 2: //Flee
			Flee(currentEntity, aiComponent);
			break;
		case 3: //Attack
			Attack(currentEntity, aiComponent);
			break;
		}
	}
}

void AISystem::PassiveStateChanges(Entity* currentEntity, AIComponent* aiComponent)
{
	if (/*Attacked*/nullptr)
	{
		aiComponent->state = 2; //Flee
	}
	else if (/*Hasn't been attacked for 10 seconds or certain distance from player*/nullptr)
	{
		aiComponent->state = 1; //Idle
	}
}

void AISystem::NeutralStateChanges(Entity* currentEntity, AIComponent* aiComponent)
{
	if (/*Attacked*/nullptr)
	{
		aiComponent->target = nullptr; //Aggressor
		aiComponent->state = 3; //Attack
	}
	else if (/*Certain distance from player*/nullptr)
	{
		aiComponent->state = 1; //Idle
	}
}

void AISystem::HostileStateChanges(Entity* currentEntity, AIComponent* aiComponent)
{
	if (/*Attacked*/nullptr)
	{
		aiComponent->target = nullptr; //Aggressor
		aiComponent->state = 3; //Attack
	}
	else if (/*In-range of player*/nullptr)
	{
		aiComponent->target = nullptr; //Target
		aiComponent->state = 3; //Attack
	}
	else if (/*Certain distance from player*/nullptr)
	{
		aiComponent->state = 1; //Idle
	}
}

void AISystem::Idle(Entity* currentEntity, AIComponent* aiComponent)
{
	//Wander
}

void AISystem::Attack(Entity* currentEntity, AIComponent* aiComponent)
{
	//Attack player
}

void AISystem::Flee(Entity* currentEntity, AIComponent* aiComponent)
{
	//Run away
}

Entity* AISystem::GetTarget(Entity* currentEntity, AIComponent* aiComponent)
{
	aiComponent = currentEntity->GetComponent<AIComponent>();
	return aiComponent->target;
}

void AISystem::SelectTarget(Entity* currentEntity, AIComponent* aiComponent, CircleColliderComponent* circleComponent)
{
	circleComponent = currentEntity->GetComponent<CircleColliderComponent>();
}