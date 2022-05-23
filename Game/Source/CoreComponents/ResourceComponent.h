#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

using namespace Firelight::Serialisation;

struct ItemDrops
{
	int itemID;
	int chance;
	int minDrop;
	int maxDrop;
};


struct ResourceComponent : Firelight::ECS::BaseComponent
{
	std::vector<ItemDrops> itemDrops;

	int resourceID;
	std::string name;

	void Serialise() override
	{
	}

	ResourceComponent* Clone() override
	{
		ResourceComponent* clone = new ResourceComponent();

		clone->itemDrops = itemDrops;
		clone->resourceID = resourceID;
		clone->name = name;
		return clone;
	}


	void RenderDebugUI() override
	{
	}
};