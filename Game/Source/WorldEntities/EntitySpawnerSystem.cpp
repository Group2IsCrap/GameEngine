#include "EntitySpawnerSystem.h"

#include "EntitySpawnerComponent.h"
#include "../Core/ResourceEntity.h"
#include "ResourceDatabase.h"
#include "../AI/Enemies/AICrocodileEntity.h"
#include "../AI/Enemies/AISlimeEntity.h"
#include "../AI/Enemies/AIDeerEntity.h"
#include "../AI/Enemies/AIBunnyEntity.h"
#include "../AI/Enemies/AIBearEntity.h"
#include "../AI/Enemies/AISnowBunnyEntity.h"
#include "../AI/Enemies/AISnowBearEntity.h"
#include <Source/ECS/EntityWrappers/SpriteEntityTemplate.h>
#include "../Core/Layers.h"
#include <Source/Graphics/AssetManager.h>
#include <Source/ECS/Components/AnimationComponent.h>
#include "../AI/AIBehaviourComponent.h"
#include "../Events/PlayerEvents.h"

using namespace Firelight::ECS;

EntitySpawnerSystem::EntitySpawnerSystem()
{
	AddWhitelistComponent<EntitySpawnerComponent>();

	Firelight::Events::EventDispatcher::SubscribeFunction(Firelight::Events::PlayerEvents::OnPlayerCreated::sm_descriptor, std::bind(&EntitySpawnerSystem::SetPlayer, this, std::placeholders::_1));

	SetupEntityTemplate();
}

void EntitySpawnerSystem::SetPlayer(void* data)
{
	m_playerEntity = new PlayerEntity((EntityID)data);
}

void EntitySpawnerSystem::Update(const Firelight::Utils::Time& time, const bool& isPaused)
{
	if (isPaused)
	{
		return;
	}

	for (size_t entityIndex = 0; entityIndex < m_entities.size(); ++entityIndex)
	{
		Entity currentEntity = Entity(m_entities[entityIndex]->GetEntityID());
		EntitySpawnerComponent* entitySpawnerComponent = currentEntity.GetComponent<EntitySpawnerComponent>();

		if (entitySpawnerComponent->spawnedEntity != nullptr)
		{
			AIComponent* aiComponent = entitySpawnerComponent->spawnedEntity->GetComponent<AIComponent>();
			if (aiComponent != nullptr)
			{
				if (aiComponent->isDead)
				{
					entitySpawnerComponent->spawnedEntity->Destroy();
					entitySpawnerComponent->spawnedEntity = nullptr;
					continue;
				}
			}

			ResourceComponent* resourceComponent = entitySpawnerComponent->spawnedEntity->GetComponent<ResourceComponent>();
			if (resourceComponent != nullptr)
			{
				if (resourceComponent->isDead)
				{
					entitySpawnerComponent->spawnedEntity->Destroy();
					entitySpawnerComponent->spawnedEntity = nullptr;
					continue;
				}
			}
		}
		
		if (Firelight::Maths::Vec3f::Dist(m_playerEntity->GetTransformComponent()->GetPosition(), currentEntity.GetComponent<TransformComponent>()->GetPosition()) > entitySpawnerComponent->spawnRadiusThreshold)
		{
			if (entitySpawnerComponent->spawnedEntity != nullptr)
			{
				if (entitySpawnerComponent->spawnedEntity->GetComponent<ResourceComponent>() != nullptr)
				{
					if (entitySpawnerComponent->resourceID != -1)
					{
						entitySpawnerComponent->initialSpawn = false;
						entitySpawnerComponent->spawnedEntity->Destroy();
						entitySpawnerComponent->spawnedEntity = nullptr;
						continue;
					}
				}
			}
			continue;
		}
		

		if (entitySpawnerComponent->spawnedEntity != nullptr)
		{
			continue;
		}

		entitySpawnerComponent->respawnTimer += time.GetDeltaTime();
		if (entitySpawnerComponent->respawnTimer >= entitySpawnerComponent->respawnCooldown || !entitySpawnerComponent->initialSpawn)
		{
			entitySpawnerComponent->respawnTimer = 0.0f;
			entitySpawnerComponent->initialSpawn = true;
			
			Firelight::Maths::Vec3f spawnPos = currentEntity.GetComponent<TransformComponent>()->GetPosition();
			if (entitySpawnerComponent->enemyName != "")
			{
				// Spawn new enemy
				if (entitySpawnerComponent->enemyName == "Crocodile")
				{
					AICrocodileEntity* croc = new AICrocodileEntity(true, enemyTemplate->GetTemplateID());
					entitySpawnerComponent->spawnedEntity = croc;
					croc->Warp(spawnPos);
					croc->GetSpriteComponent()->layer += croc->GetTransformComponent()->GetPosition().y;
				}
				else if (entitySpawnerComponent->enemyName == "Slime")
				{
					AISlimeEntity* slime = new AISlimeEntity(true, enemyTemplate->GetTemplateID());
					entitySpawnerComponent->spawnedEntity = slime;
					slime->Warp(spawnPos);
					slime->GetSpriteComponent()->layer += slime->GetTransformComponent()->GetPosition().y;
				}
				else if (entitySpawnerComponent->enemyName == "Deer")
				{
					AIDeerEntity* deer = new AIDeerEntity(true, enemyTemplate->GetTemplateID());
					entitySpawnerComponent->spawnedEntity = deer;
					deer->Warp(spawnPos);
					deer->GetSpriteComponent()->layer += deer->GetTransformComponent()->GetPosition().y;
				}
				else if (entitySpawnerComponent->enemyName == "Bunny")
				{
					AIBunnyEntity* bunny = new AIBunnyEntity(true, enemyTemplate->GetTemplateID());
					entitySpawnerComponent->spawnedEntity = bunny;
					bunny->Warp(spawnPos);
					bunny->GetSpriteComponent()->layer += bunny->GetTransformComponent()->GetPosition().y;
				}
				else if (entitySpawnerComponent->enemyName == "Bear")
				{
					AIBearEntity* bear = new AIBearEntity(true, enemyTemplate->GetTemplateID());
					entitySpawnerComponent->spawnedEntity = bear;
					bear->Warp(spawnPos);
					bear->GetSpriteComponent()->layer += bear->GetTransformComponent()->GetPosition().y;
				}
				else if (entitySpawnerComponent->enemyName == "SnowBear")
				{
					AISnowBearEntity* snowBear = new AISnowBearEntity(true, enemyTemplate->GetTemplateID());
					entitySpawnerComponent->spawnedEntity = snowBear;
					snowBear->Warp(spawnPos);
					snowBear->GetSpriteComponent()->layer += snowBear->GetTransformComponent()->GetPosition().y;
				}
				else if (entitySpawnerComponent->enemyName == "SnowBunny")
				{
					AISnowBunnyEntity* snowBunny = new AISnowBunnyEntity(true, enemyTemplate->GetTemplateID());
					entitySpawnerComponent->spawnedEntity = snowBunny;
					snowBunny->Warp(spawnPos);
					snowBunny->GetSpriteComponent()->layer += snowBunny->GetTransformComponent()->GetPosition().y;
				}
			}
			else if (entitySpawnerComponent->resourceID != -1)
			{
				// Spawn new resource
				ResourceEntity* resourceEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(entitySpawnerComponent->resourceID);
				entitySpawnerComponent->spawnedEntity = resourceEntity;
				resourceEntity->Warp(spawnPos);
				resourceEntity->GetSpriteComponent()->layer += resourceEntity->GetTransformComponent()->GetPosition().y;
			}
		}
	}
}

void EntitySpawnerSystem::SetupEntityTemplate()
{
	enemyTemplate = new SpriteEntityTemplate("Enemy Template");
	AIComponent* aiComponent = enemyTemplate->AddComponent<AIComponent>();
	enemyTemplate->GetComponent<LayerComponent>()->layer = static_cast<int>(GameLayer::Enemy);
	SpriteComponent* spriteComponent = enemyTemplate->GetComponent<SpriteComponent>();
	spriteComponent->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Enemies/DeerIdle.png");
	spriteComponent->pixelsPerUnit = 50;
	spriteComponent->layer = static_cast<int>(RenderLayer::Enemy);
	enemyTemplate->AddComponent<RigidBodyComponent>();
	enemyTemplate->AddComponent<AIBehaviourComponent>();
	enemyTemplate->AddComponent<HealthComponent>();
	enemyTemplate->AddComponent<Firelight::ECS::AnimationComponent>();
	AudioComponent* audioComponent = new AudioComponent();
	enemyTemplate->AddComponent<Firelight::ECS::AudioComponent>(audioComponent);
	audioComponent->looping = false;
	audioComponent->is3d = false;
	audioComponent->streaming = false;
	audioComponent->channel = "Enemies";
}

void EntitySpawnerSystem::SpawnEnemy()
{
}

void EntitySpawnerSystem::SpawnResource()
{
}
