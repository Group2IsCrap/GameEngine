#include "ResourceEntity.h"
#include "../Items/ItemDatabase.h"

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
		savePos = GetComponent<TransformComponent>()->GetPosition();
		component->currentHealth = 0;
		HealthBelowZero();
	}
}

void ResourceEntity::HealthBelowZero()
{
	//Determine what to drop
	//Loop and call drop items
	ResourceComponent* resourceComponent = GetComponent<ResourceComponent>();
	for (int i = 0; i < resourceComponent->itemDrops.size(); i++)
	{
		ItemDrops drop = resourceComponent->itemDrops[i];
		for (int j = 0; j < drop.minDrop; j++)
		{
			DropItems(ItemDatabase::Instance()->CreateInstanceOfItem(drop.itemID)->GetEntityID(), GetTransformComponent()->GetPosition());
		}
		for (int k = 0; k < drop.maxDrop - drop.minDrop; k++)
		{
			int roll = Firelight::Maths::Random::RandomRange<int>(0, 100);
			if (roll <= drop.chance)
			{
				DropItems(ItemDatabase::Instance()->CreateInstanceOfItem(drop.itemID)->GetEntityID(), GetTransformComponent()->GetPosition());
			}
		}
	}

	this->GetComponent<ResourceComponent>()->isDead = true;
}

void ResourceEntity::DropItems(Firelight::ECS::EntityID drop, Firelight::Maths::Vec3f location)
{
	Firelight::ECS::TransformComponent* toDropData = Firelight::ECS::EntityComponentSystem::Instance()->GetComponent<Firelight::ECS::TransformComponent>(drop);
	if (toDropData)
	{
		toDropData->SetPosition(Firelight::Maths::Random::RandomPointInCircle(location, 3));
	}
}
