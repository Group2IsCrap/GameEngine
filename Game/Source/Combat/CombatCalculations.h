#pragma once
#include <cstdlib>
#include <Source/Maths/Vec3.h>
#include <Source/Physics/PhysicsHelpers.h>
#include "../Player/PlayerComponent.h"
#include "../Core/Layers.h"
#include "../Player/PlayerEntity.h"

#include <Source/ECS/Components/ItemComponents.h>

using namespace Firelight::Maths;
using namespace Firelight::Physics;



class CombatCalculations
{
public:
	static void PlaceSphere(Facing dir, Vec3f Point, PlayerEntity* m_player);

	static void ChangeWeapon(WeaponComponent* component);

private:
	static WeaponComponent* activeComponent;
	
};