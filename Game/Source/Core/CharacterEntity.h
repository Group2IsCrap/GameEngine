#pragma once

#include <Source/ECS/EntityWrappers/SpriteEntity.h>
#include <Source/ECS/Components/PhysicsComponents.h>

#include "../CoreComponents/HealthComponent.h"
#include "../Inventory/InventoryEntity.h"
class CharacterEntity : public Firelight::ECS::SpriteEntity
{
public:

	CharacterEntity();
	CharacterEntity(std::string name);
	CharacterEntity(Firelight::ECS::EntityID entityID);
	CharacterEntity(bool isTemplate, Firelight::ECS::EntityID entityID);

	HealthComponent* GetHealthComponent();
	Firelight::ECS::RigidBodyComponent* GetRigidBodyComponent();
	int GetHealth();
	int GetMaxHealth();
	virtual void RemoveHealth(int amount);
	virtual void AddHealth(int amount);
	virtual void HealthBelowZero();
};

