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
	float speed = 5.5f;
	Facing facing;
	Entity* weapon;
	Entity* weaponSocket;
	Entity* hatSocket;
	Entity* hat;
	Entity* bodySocket;
	Entity* body;

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

	void RenderDebugUI() override
	{
		ImGuiVariable("Name", name);
		ImGuiVariable("Speed", speed);
		ImGuiVariable("Facing", (int)facing);
	}
};
