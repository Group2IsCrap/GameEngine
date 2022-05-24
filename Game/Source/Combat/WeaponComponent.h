#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>

# include <string>

using namespace Firelight::Serialisation;

struct WeaponComponent : Firelight::ECS::BaseComponent
{
	float Cooldown = 1;
	float Damage = 5;
	float Radius = 0;
	float Angle = 40;

	void Serialise() override
	{
		/*Serialiser::Serialise("WeaponSpeed", WeaponSpeed);
		Serialiser::Serialise("WeaponDamage", WeaponDamage);*/
	}

	WeaponComponent* Clone() override
	{
		/*WeaponComponent* clone = new WeaponComponent();
		clone->WeaponDamage = WeaponDamage;
		clone->WeaponName = WeaponName;
		clone->WeaponSpeed = WeaponSpeed;
		return clone;*/
	}
};