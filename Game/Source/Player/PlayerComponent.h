#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

#include <string>

using namespace Firelight::Serialisation;

struct PlayerComponent : Firelight::ECS::BaseComponent
{
	std::string name = "";

	void Serialise() override
	{
		Serialiser::Serialise("Name", name.c_str());
	}

	PlayerComponent* Clone() override
	{
		PlayerComponent* clone = new PlayerComponent();
		clone->name = name;
		return clone;
	}
};
