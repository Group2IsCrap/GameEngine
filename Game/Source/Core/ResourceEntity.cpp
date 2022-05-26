#include "ResourceEntity.h"

#include "../Items/ItemDatabase.h"

ResourceEntity::ResourceEntity()
{
	GetIDComponent()->name = "Resource";
}

ResourceEntity::ResourceEntity(std::string name) : ResourceEntity()
{
	GetIDComponent()->name = name;
}

ResourceEntity::ResourceEntity(Firelight::ECS::EntityID entityID) : CharacterEntity(entityID)
{

}

ResourceEntity::ResourceEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : CharacterEntity(isTemplate, entityID)
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
	AudioComponent* audioComponent = this->GetComponent<AudioComponent>();
	component->currentHealth -= amount;
	if (component->currentHealth <= 0)
	{
		component->currentHealth = 0;
		HealthBelowZero();
	}
	else
	{
		switch (GetComponent<ResourceComponent>()->resourceID)
		{
		case 0:
			switch (Firelight::Maths::Random::RandomRange(0, 1))
			{
			case 0:
				audioComponent->soundName = "Wood Chop 1.wav";
				break;
			case 1:
				audioComponent->soundName = "Wood Chop 2.wav";
				break;
			}
			break;
		case 1:
			switch (Firelight::Maths::Random::RandomRange(0, 1))
			{
			case 0:
				audioComponent->soundName = "Mining.wav";
				break;
			case 1:
				audioComponent->soundName = "Mining 2.wav";
				break;
			}
			break;
		case 2:
			audioComponent->soundName = "Bush Harvest.wav";
			break;
		case 3:
			audioComponent->soundName = "Bush Harvest.wav";
			break;
		}

		audioComponent->soundPos = Vector3D(GetTransformComponent()->GetPosition().x, GetTransformComponent()->GetPosition().y, GetTransformComponent()->GetPosition().z);

		this->PlayAudioClip();
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

	switch (resourceComponent->resourceID)
	{
	case 0:
		GetComponent<AudioComponent>()->soundName = "Tree Destroyed.wav";
		break;
	case 1:
		GetComponent<AudioComponent>()->soundName = "Rock Breaking.wav";
	}

	GetComponent<AudioComponent>()->soundPos = Vector3D(GetTransformComponent()->GetPosition().x, GetTransformComponent()->GetPosition().y, GetTransformComponent()->GetPosition().z);

	this->PlayAudioClip();

	if (resourceComponent->resourceID >= 5)
	{
		this->Destroy();
	}
	else
	{
		this->GetComponent<ResourceComponent>()->isDead = true;
	}


	
}

void ResourceEntity::DropItems(Firelight::ECS::EntityID drop, Firelight::Maths::Vec3f location)
{
	Firelight::ECS::TransformComponent* toDropData = Firelight::ECS::EntityComponentSystem::Instance()->GetComponent<Firelight::ECS::TransformComponent>(drop);
	if (toDropData)
	{
		toDropData->SetPosition(Firelight::Maths::Random::RandomPointInCircle(location, 3));
	}
}
