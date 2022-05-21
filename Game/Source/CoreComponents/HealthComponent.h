#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

using namespace Firelight::Serialisation;

struct HealthComponent : Firelight::ECS::BaseComponent
{
	int currentHealth = 1;
	int maxHealth = 1;

	void Serialise() override
	{
		Serialiser::Serialise("CurrentHealth", currentHealth);
		Serialiser::Serialise("MaxHealth", maxHealth);
	}

	HealthComponent* Clone() override
	{
		HealthComponent* clone = new HealthComponent();
		clone->currentHealth = currentHealth;
		clone->maxHealth = maxHealth;
		return clone;
	}
	void RenderDebugUI() override
	{
		//ImGuiVariable("CurrentHealth", currentHealth);
		//ImGuiVariable("MaxHealth", maxHealth);
	}
};