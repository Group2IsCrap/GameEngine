#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>
#include <Source/Physics/PhysicsHelpers.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"
#include "../Items/ItemDatabase.h"
#include "../Core/Layers.h"


PlayerSystem::PlayerSystem()
{
	AddWhitelistComponent<PlayerComponent>();
	playerEntity = nullptr;

	m_playerEntityAddedCheckIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::ECS::OnEntityCreatedEvent>(std::bind(&PlayerSystem::CheckForPlayer, this));
	m_playerMoveUpIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveUpEvent>(std::bind(&PlayerSystem::MovePlayerUp, this));
	m_playerMoveLeftIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveLeftEvent>(std::bind(&PlayerSystem::MovePlayerLeft, this));
	m_playerMoveRightIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveDownEvent>(std::bind(&PlayerSystem::MovePlayerDown, this));
	m_playerMoveDownIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveRightEvent>(std::bind(&PlayerSystem::MovePlayerRight, this));
	m_interactionEventIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnInteractEvent>(std::bind(&PlayerSystem::Interact, this));
	m_spawnItemEventIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::SpawnItemEvent>(std::bind(&PlayerSystem::SpawnItem, this));
	m_removeHealthEventIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::RemoveHealthEvent>(std::bind(&PlayerSystem::RemoveHealth, this));

	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::InputEvents::OnPlayerMoveEvent>(this);
}

PlayerSystem::~PlayerSystem()
{
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::ECS::OnEntityCreatedEvent>(m_playerEntityAddedCheckIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveUpEvent>(m_playerMoveUpIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveLeftEvent>(m_playerMoveLeftIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveDownEvent>(m_playerMoveRightIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveRightEvent>(m_playerMoveDownIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnInteractEvent>(m_interactionEventIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::SpawnItemEvent>(m_spawnItemEventIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::RemoveHealthEvent>(m_removeHealthEventIndex);
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

void PlayerSystem::RemoveHealth()
{
	playerEntity->RemoveHealth(1);
}

