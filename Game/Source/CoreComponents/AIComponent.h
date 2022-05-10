#pragma once
#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

using namespace Firelight::Serialisation;
using namespace Firelight::ECS;

struct AIComponent : Firelight::ECS::BaseComponent
{
	int state; //Idle = 1, Flee = 2, Attack = 3
	int hostility; //Passive = 1, Neutral = 2, Hostile = 3
	
	//Target
	Entity* target;


	void Serialise() override
	{
		Serialiser::Serialise("State", state);
		Serialiser::Serialise("hostility", hostility);
		Serialiser::Serialise("target", target);
	}

	AIComponent* Clone() override
	{
		AIComponent* clone = new AIComponent();
		clone->state = state;
		clone->hostility = hostility;
		clone->target = target;

		return clone;
	}
};