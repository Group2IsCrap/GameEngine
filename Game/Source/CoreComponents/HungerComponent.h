#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

using namespace Firelight::Serialisation;

struct HungerComponent : Firelight::ECS::BaseComponent
{
	int currentHunger = 1;
	int startHunger = 1;
	int maxHunger = 1;
	int hungerDecreaseAmount = 1;
	float hungerDecreaseInterval = 5.0f;

	void Serialise() override
	{
		Serialiser::Serialise("CurrentHunger", currentHunger);
		Serialiser::Serialise("StartHunger", startHunger);
		Serialiser::Serialise("MaxHunger", maxHunger);
		Serialiser::Serialise("HungerDecreaseAmount", hungerDecreaseAmount);
		Serialiser::Serialise("HungerDecreaseInterval", hungerDecreaseInterval);
	}

	HungerComponent* Clone() override
	{
		HungerComponent* clone = new HungerComponent();
		clone->currentHunger = currentHunger;
		clone->startHunger = startHunger;
		clone->maxHunger = maxHunger;
		clone->hungerDecreaseAmount = hungerDecreaseAmount;
		clone->hungerDecreaseInterval = hungerDecreaseInterval;
		return clone;
	}

	void RenderDebugUI() override
	{
		ImGuiVariable("CurrentHunger", currentHunger);
		ImGuiVariable("StartHunger", startHunger);
		ImGuiVariable("MaxHunger", maxHunger);
		ImGuiVariable("HungerDecreaseAmount", hungerDecreaseAmount);
		ImGuiVariable("HungerDecreaseInterval", hungerDecreaseInterval);
	}
};
