#include "CharacterEntity.h"

CharacterEntity::CharacterEntity()
{
	AddComponent<Firelight::ECS::RigidBodyComponent>();
	AddComponent<HealthComponent>();
}

CharacterEntity::CharacterEntity(Firelight::ECS::EntityID entityID) : Firelight::ECS::SpriteEntity(entityID)
{

}

CharacterEntity::CharacterEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : SpriteEntity(isTemplate, entityID)
{
}

HealthComponent* CharacterEntity::GetHealthComponent()
{
	return GetComponent<HealthComponent>();
}

Firelight::ECS::RigidBodyComponent* CharacterEntity::GetRigidBodyComponent()
{
	return GetComponent<Firelight::ECS::RigidBodyComponent>();
}

int CharacterEntity::GetHealth()
{
	return GetComponent<HealthComponent>()->currentHealth;
}

int CharacterEntity::GetMaxHealth()
{
	return GetHealthComponent()->maxHealth;
}

void CharacterEntity::RemoveHealth(int amount)
{
	HealthComponent* component = GetComponent<HealthComponent>();
	component->currentHealth -= amount;
	if (component->currentHealth <= 0)
	{
		component->currentHealth = 0;
		HealthBelowZero();
	}
}

void CharacterEntity::HealthBelowZero()
{
}
