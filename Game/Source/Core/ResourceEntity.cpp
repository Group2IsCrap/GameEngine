#include "ResourceEntity.h"

ResourceEntity::ResourceEntity()
{
	AddComponent<Firelight::ECS::RigidBodyComponent>();
	AddComponent<HealthComponent>();
	this->GetIDComponent()->name = "Resource";
}

ResourceEntity::ResourceEntity(std::string name) : ResourceEntity()
{
	this->GetIDComponent()->name = name;
}

ResourceEntity::ResourceEntity(Firelight::ECS::EntityID entityID) : Firelight::ECS::SpriteEntity(entityID)
{

}

ResourceEntity::ResourceEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : SpriteEntity(isTemplate, entityID)
{
}

HealthComponent* ResourceEntity::GetHealthComponent()
{
	return GetComponent<HealthComponent>();
}

Firelight::ECS::RigidBodyComponent* ResourceEntity::GetRigidBodyComponent()
{
	return GetComponent<Firelight::ECS::RigidBodyComponent>();
}

int ResourceEntity::GetHealth()
{
	return GetComponent<HealthComponent>()->currentHealth;
}

void ResourceEntity::RemoveHealth(int amount)
{
	HealthComponent* component = GetComponent<HealthComponent>();
	component->currentHealth -= amount;
	if (component->currentHealth <= 0)
	{
		component->currentHealth = 0;
		HealthBelowZero();
	}
}

void ResourceEntity::HealthBelowZero()
{
	Firelight::ECS::EntityComponentSystem::Instance()->RemoveEntity(GetEntityID());
}