#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>
#include <Source/Physics/PhysicsHelpers.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"
#include "../Items/ItemDatabase.h"
#include "../Core/Layers.h"

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

void PlayerSystem::FixedUpdate(const Firelight::Utils::Time& time)
{
	playerEntity->GetRigidBodyComponent()->velocity += m_velocity;
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
	m_moveUp = true;
	//m_velocity.y += playerEntity->GetComponent<PlayerComponent>()->speed;
	//playerEntity->GetRigidBodyComponent()->velocity.y += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerLeft()
{
	m_velocity.x -= playerEntity->GetComponent<PlayerComponent>()->speed;
	//playerEntity->GetRigidBodyComponent()->velocity.x -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);

}
void PlayerSystem::MovePlayerDown()
{
	m_velocity.y -= playerEntity->GetComponent<PlayerComponent>()->speed;
	//playerEntity->GetRigidBodyComponent()->velocity.y -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);

}
void PlayerSystem::MovePlayerRight()
{
	m_velocity.x += playerEntity->GetComponent<PlayerComponent>()->speed;
	//playerEntity->GetRigidBodyComponent()->velocity.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);

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

