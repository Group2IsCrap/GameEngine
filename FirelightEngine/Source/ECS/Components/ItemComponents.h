#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../../Maths/Vec3.h"
#include "../../Serialisation/Serialiser.h"

using namespace Firelight::Serialisation;

namespace Firelight::ECS
{
	/// <summary>
	/// Simple ID component used for testing
	/// </summary>
	struct ItemComponent : BaseComponent
	{
		int itemID;
		std::string name;
		std::string description;
		std::string iconPath;
		int stackSize;
		std::vector<std::string> tags;

		void Serialise() override
		{
			Serialiser::Serialise("ItemID", itemID);
			Serialiser::Serialise("Name", name.c_str());
			Serialiser::Serialise("Description", description);
			Serialiser::Serialise("IconPath", iconPath);
			Serialiser::Serialise("StackSize", stackSize);
		}

		ItemComponent* Clone() override
		{
			ItemComponent* clone = new ItemComponent();
			clone->itemID = itemID;
			clone->name = name;
			clone->description = description;
			clone->iconPath = iconPath;
			clone->stackSize = stackSize;
			clone->tags = tags;

			return clone;
		}
	};
}