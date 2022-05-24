#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../../Maths/Vec3.h"
#include "../../Serialisation/Serialiser.h"

using namespace Firelight::Serialisation;

namespace Firelight::ECS
{

	enum class e_HarvestType {
		Tree,
		Rock,
		invalid
	};

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

		void RenderDebugUI() override
		{
			ImGuiVariable("ItemID", itemID);
			ImGuiVariable("Name", name);
			ImGuiVariable("Description", description);
			ImGuiVariable("IconPath", iconPath);
			ImGuiVariable("StackSize", stackSize);
			ImGuiText("Tags");
			for (int i = 0; i < tags.size(); ++i)
			{
				ImGuiVariable("    " + std::to_string(i), tags[i]);
			}
		}
	};

	struct FoodComponent : BaseComponent
	{
		int HealthRegeneration = 0;

		void Serialise() override
		{
			Serialiser::Serialise("HealthRegeneration", HealthRegeneration);
			
		}

		FoodComponent* Clone() override
		{
			FoodComponent* clone = new FoodComponent();
			clone->HealthRegeneration = HealthRegeneration;
			return clone;
		}
	};


	struct WeaponComponent : Firelight::ECS::BaseComponent
	{
		float Cooldown = 1;
		float Damage = 5;
		float Radius = 0;
		float Angle = 40;
		e_HarvestType HarvestType = e_HarvestType::invalid;
		float HarvestDamage = 0;

		void Serialise() override
		{
			Serialiser::Serialise("Damage", Damage);
			Serialiser::Serialise("Cooldown", Cooldown);
			Serialiser::Serialise("Radius", Radius);
			Serialiser::Serialise("Angle", Angle);
			
		}

		WeaponComponent* Clone() override
		{
			WeaponComponent* clone = new WeaponComponent();
			clone->Damage = Damage;
			clone->Cooldown = Cooldown;
			clone->Radius = Radius;
			clone->Angle = Angle;
			clone->HarvestType = HarvestType;
			clone->HarvestDamage=HarvestDamage;
			return clone;
		}
	};


	struct ArmourComponent : Firelight::ECS::BaseComponent
	{
		float armourValue = 1;
		float durability = 5;
		

		void Serialise() override
		{
			Serialiser::Serialise("value", armourValue);
			Serialiser::Serialise("durability", durability);
		}

		ArmourComponent* Clone() override
		{
			ArmourComponent* clone = new ArmourComponent();
			clone->armourValue = armourValue;
			clone->durability = durability;
			
			return clone;
		}
	};

}