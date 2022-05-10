#pragma once
#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

using namespace Firelight::Serialisation;

struct AIComponent : Firelight::ECS::BaseComponent
{
	int state; //Idle = 1, Run = 2, Attack = 3
	int hostility; //Passive = 1, Neutral = 2, Hostile = 3


	void Serialise() override
	{
		Serialiser::Serialise("State", state);
		Serialiser::Serialise("hostility", hostility);
	}

	AIComponent* Clone() override
	{
		AIComponent* clone = new AIComponent();
		clone->state = state;
		clone->hostility = hostility;

		return clone;
	}
};