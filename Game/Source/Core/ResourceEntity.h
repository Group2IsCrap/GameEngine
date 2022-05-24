#pragma once

#include <Source/ECS/EntityWrappers/SpriteEntity.h>
#include <Source/ECS/Components/PhysicsComponents.h>

#include "../CoreComponents/HealthComponent.h"
#include "../CoreComponents/ResourceComponent.h"

#include <Source/ECS/ECSDefines.h>
#include <Source/Maths/Vec3.h>

class ResourceEntity : public Firelight::ECS::SpriteEntity
{
public:

	ResourceEntity();
	ResourceEntity(std::string name);
	ResourceEntity(Firelight::ECS::EntityID entityID);
	ResourceEntity(bool isTemplate, Firelight::ECS::EntityID entityID);

	HealthComponent* GetHealthComponent();
	Firelight::ECS::RigidBodyComponent* GetRigidBodyComponent();
	int GetHealth();
	virtual void RemoveHealth(int amount);
	virtual void HealthBelowZero();

	void DropItems(Firelight::ECS::EntityID drop, Firelight::Maths::Vec3f location);

	
};
