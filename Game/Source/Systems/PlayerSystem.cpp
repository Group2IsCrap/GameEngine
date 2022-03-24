#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>
#include <Source/Physics/PhysicsHelpers.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"
#include "../Events/InputEvents.h"

static bool s_triggered = false;

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

	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnInteractEvent::sm_descriptor, Firelight::Keys::KEY_F);
}

PlayerSystem::~PlayerSystem()
{
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::ECS::OnEntityCreatedEvent>(m_playerEntityAddedCheckIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveUpEvent>(m_playerMoveUpIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveLeftEvent>(m_playerMoveLeftIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveDownEvent>(m_playerMoveRightIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveRightEvent>(m_playerMoveDownIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnInteractEvent>(m_interactionEventIndex);
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

	if (Firelight::Input::InputGet.KeyIsPress('K') && !s_triggered)
	{
		s_triggered = true;
		playerEntity->RemoveHealth(1);
	}
}

void PlayerSystem::MovePlayerUp()
{
	playerEntity->GetTransformComponent()->position.y += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerLeft()
{
	playerEntity->GetTransformComponent()->position.x -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerDown()
{
	playerEntity->GetTransformComponent()->position.y -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerRight()
{
	playerEntity->GetTransformComponent()->position.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}

void PlayerSystem::Interact()
{
	std::vector<Firelight::ECS::Entity*> entitiesCollidedWith = Firelight::Physics::PhysicsHelpers::OverlapCircle(playerEntity->GetTransformComponent()->position, 1.0f, 1);
	if (entitiesCollidedWith.size() > 0)
	{
		playerEntity->GetTransformComponent()->position.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
	}
}
