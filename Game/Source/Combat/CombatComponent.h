#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

# include <string>

using namespace Firelight::Serialisation;

struct CombatComponent : Firelight::ECS::BaseComponent
{
	// data for combat componant
	std::string WeaponName = "";
	float WeaponSpeed = 1;
	float WeaponDamage = 5;

	void Serialise() override
	{
		Serialiser::Serialise("WeaponName", WeaponName.c_str());
		Serialiser::Serialise("WeaponSpeed", WeaponSpeed);
		Serialiser::Serialise("WeaponDamage", WeaponDamage);
	}

	CombatComponent* Clone() override
	{
		CombatComponent* clone = new CombatComponent();
		clone->WeaponDamage = WeaponDamage;
		clone->WeaponName = WeaponName;
		clone->WeaponSpeed = WeaponSpeed;
		return clone;
	}
};