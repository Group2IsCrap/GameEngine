#include "CharacterEntity.h"

CharacterEntity::CharacterEntity() : Firelight::ECS::SpriteEntity()
{
	AddComponent<Firelight::ECS::RigidBodyComponent>();
	AddComponent<HealthComponent>();
	this->GetIDComponent()->name = "Character Entity";
}


CharacterEntity::CharacterEntity(std::string name) : CharacterEntity()
{
	this->GetIDComponent()->name = name;
}

CharacterEntity::CharacterEntity(Firelight::ECS::EntityID entityID) : Firelight::ECS::SpriteEntity(entityID)
{

}

CharacterEntity::CharacterEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : SpriteEntity(isTemplate, entityID)
{
}

void CharacterEntity::Warp(Firelight::Maths::Vec3f pos)
{
	RigidBodyComponent* rb = GetRigidBodyComponent();
	rb->nextPos = pos;
	rb->lastPos = pos;
	rb->velocity = Firelight::Maths::Vec3f(0.0f, 0.0f, 0.0f);
	GetTransformComponent()->SetPosition(pos);
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

void CharacterEntity::AddHealth(int amount)
{
	HealthComponent* component = GetComponent<HealthComponent>();
	if (component->maxHealth >= amount)
	{
		component->currentHealth += amount;
	}
}

void CharacterEntity::HealthBelowZero()
{
}
