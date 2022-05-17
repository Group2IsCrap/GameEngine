#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

#include <string>

using namespace Firelight::Serialisation;


enum class Facing
{
	Left,
	Right,
	Up,
	Down
};

struct PlayerComponent : Firelight::ECS::BaseComponent
{
	std::string name = "";
	float speed = 1.5f;
	Facing facing;

	void Serialise() override
	{
		Serialiser::Serialise("Name", name.c_str());
		Serialiser::Serialise("Speed", speed);
	}

	PlayerComponent* Clone() override
	{
		PlayerComponent* clone = new PlayerComponent();
		clone->name = name;
		clone->speed = speed;
		return clone;
	}
};
