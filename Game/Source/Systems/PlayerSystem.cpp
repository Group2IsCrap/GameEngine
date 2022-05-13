#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>
#include <Source/Physics/PhysicsHelpers.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"
#include "../Items/ItemDatabase.h"
#include "../Core/Layers.h"
#include "../Core/CharacterEntity.h"
#include "../Combat/CombatCalculations.h"

using namespace Firelight::Events;
using namespace Firelight::Events::InputEvents;

PlayerSystem::PlayerSystem()
{
	AddWhitelistComponent<PlayerComponent>();
	playerEntity = nullptr;

	m_playerEntityAddedCheckIndex = EventDispatcher::SubscribeFunction<ECS::OnEntityCreatedEvent>(std::bind(&PlayerSystem::CheckForPlayer, this));
	m_playerMoveUpIndex = EventDispatcher::SubscribeFunction<OnPlayerMoveUpEvent>(std::bind(&PlayerSystem::MovePlayerUp, this));
	m_playerMoveLeftIndex = EventDispatcher::SubscribeFunction<OnPlayerMoveLeftEvent>(std::bind(&PlayerSystem::MovePlayerLeft, this));
	m_playerMoveRightIndex = EventDispatcher::SubscribeFunction<OnPlayerMoveDownEvent>(std::bind(&PlayerSystem::MovePlayerDown, this));
	m_playerMoveDownIndex = EventDispatcher::SubscribeFunction<OnPlayerMoveRightEvent>(std::bind(&PlayerSystem::MovePlayerRight, this));
	m_interactionEventIndex = EventDispatcher::SubscribeFunction<OnInteractEvent>(std::bind(&PlayerSystem::Interact, this));
	m_spawnItemEventIndex = EventDispatcher::SubscribeFunction<SpawnItemEvent>(std::bind(&PlayerSystem::SpawnItem, this));
	m_removeHealthEventIndex = EventDispatcher::SubscribeFunction<RemoveHealthEvent>(std::bind(&PlayerSystem::RemoveHealth, this));
	m_attackIndex = EventDispatcher::SubscribeFunction<AttackEvent>(std::bind(&PlayerSystem::Attack, this));

	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::InputEvents::OnPlayerMoveEvent>(this);
}

PlayerSystem::~PlayerSystem()
{
	EventDispatcher::UnsubscribeFunction<ECS::OnEntityCreatedEvent>(m_playerEntityAddedCheckIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveUpEvent>(m_playerMoveUpIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveLeftEvent>(m_playerMoveLeftIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveDownEvent>(m_playerMoveRightIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveRightEvent>(m_playerMoveDownIndex);
	EventDispatcher::UnsubscribeFunction<OnInteractEvent>(m_interactionEventIndex);
	EventDispatcher::UnsubscribeFunction<SpawnItemEvent>(m_spawnItemEventIndex);
	EventDispatcher::UnsubscribeFunction<RemoveHealthEvent>(m_removeHealthEventIndex);
	EventDispatcher::UnsubscribeFunction<AttackEvent>(m_attackIndex);
}

void PlayerSystem::CheckForPlayer()
{
	if (playerEntity == nullptr && m_entities.size() > 0)
	{
		playerEntity = new PlayerEntity(m_entities[0]->GetEntityID());
	}
}

void PlayerSystem::Update(const Firelight::Utils::Time& time)
{
	if (playerEntity == nullptr)
	{
		return;
	}
}

void PlayerSystem::HandleEvents(DescriptorType event, void* data)
{
	if (event == Firelight::Events::InputEvents::OnPlayerMoveEvent::sm_descriptor)
	{
		Firelight::Maths::Vec2f axis = *(reinterpret_cast<Firelight::Maths::Vec2f*>(data));

		playerEntity->GetTransformComponent()->position.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed) * axis.x * 2;
		playerEntity->GetTransformComponent()->position.y += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed) * axis.y * 2;
	}
}

void PlayerSystem::MovePlayerUp()
{
	playerEntity->GetRigidBodyComponent()->velocity.y += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerLeft()
{
	playerEntity->GetRigidBodyComponent()->velocity.x -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);

}
void PlayerSystem::MovePlayerDown()
{
	playerEntity->GetRigidBodyComponent()->velocity.y -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);

}
void PlayerSystem::MovePlayerRight()
{
	playerEntity->GetRigidBodyComponent()->velocity.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);

}

void PlayerSystem::Interact()
{
	std::vector<Firelight::ECS::Entity*> entitiesCollidedWith = Firelight::Physics::PhysicsHelpers::OverlapCircle(playerEntity->GetTransformComponent()->position, 1.0f, static_cast<int>(GameLayer::Items));
	if (entitiesCollidedWith.size() > 0)
	{
		playerEntity->GetTransformComponent()->position.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
	}
}

void PlayerSystem::SpawnItem()
{
	Entity* itemEntity = ItemDatabase::Instance()->CreateInstanceOfItem(0);
	itemEntity->GetComponent<TransformComponent>()->position = playerEntity->GetTransformComponent()->position;
}
void PlayerSystem::Attack()
{
	std::vector<Firelight::ECS::Entity*> entitiesinRange = Firelight::Physics::PhysicsHelpers::OverlapCircle(playerEntity->GetTransformComponent()->position, 2.0f, (int) GameLayer::Enemies);

	for (int entityIndex = 0; entityIndex < entitiesinRange.size(); ++entityIndex)
	{
		Firelight::ECS::Entity* entity = entitiesinRange[entityIndex];
		TransformComponent* transform = entity->GetComponent<TransformComponent>();

		char playerDirection;
		int pointOneX, pointOneY, pointTwoX, pointTwoY;
		if (playerEntity->GetSpriteComponent()->texture == /*player left texture*/) 
		{
			pointOneX = playerEntity->GetTransformComponent()->position.x - 5; 
			pointTwoX = playerEntity->GetTransformComponent()->position.x - 5;
			pointOneY = playerEntity->GetTransformComponent()->position.y + 3;
			pointTwoY = playerEntity->GetTransformComponent()->position.y - 3;
		}
		if (playerEntity->GetSpriteComponent()->texture == /*player up texture*/) 
		{
			pointOneX = playerEntity->GetTransformComponent()->position.x - 3;
			pointTwoX = playerEntity->GetTransformComponent()->position.x + 3;
			pointOneY = playerEntity->GetTransformComponent()->position.y + 5;
			pointTwoY = playerEntity->GetTransformComponent()->position.y + 5;
		}
		if (playerEntity->GetSpriteComponent()->texture == /*player right texture*/) 
		{
			pointOneX = playerEntity->GetTransformComponent()->position.x + 5;
			pointTwoX = playerEntity->GetTransformComponent()->position.x + 5;
			pointOneY = playerEntity->GetTransformComponent()->position.y + 3;
			pointTwoY = playerEntity->GetTransformComponent()->position.y - 3;
		}
		if (playerEntity->GetSpriteComponent()->texture == /*player down texture*/) 
		{
			pointOneX = playerEntity->GetTransformComponent()->position.x - 3;
			pointTwoX = playerEntity->GetTransformComponent()->position.x + 3;
			pointOneY = playerEntity->GetTransformComponent()->position.y - 5;
			pointTwoY = playerEntity->GetTransformComponent()->position.y - 5;
		}


		if (transform != nullptr)
		{                                                                                                                                 //two points on triangle to check depending on direction player is facing
			if (CombatCalculations::IsPointInTriangle(playerEntity->GetTransformComponent()->position.x, playerEntity->GetTransformComponent()->position.y, pointOneX, pointOneY, pointTwoX, pointTwoY, entity->GetComponent<TransformComponent>()->position.x, entity->GetComponent<TransformComponent>()->position.y))
			{
				entity->GetComponent <HealthComponent>()->currentHealth - 1; //replace 1 with damage from weapon componant when done
				entitiesinRange[entityIndex] = entity;
			}
		}
		
		//replace transform with ridgid body and next position
	}
}

void PlayerSystem::RemoveHealth()
{
	playerEntity->RemoveHealth(1);
}

