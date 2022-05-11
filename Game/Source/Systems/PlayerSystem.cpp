#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>
#include <Source/Physics/PhysicsHelpers.h>
#include <Source/ImGuiUI/ImGuiManager.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"
#include "../Items/ItemDatabase.h"

static bool s_triggered = false;

PlayerSystem::PlayerSystem()
{
	AddWhitelistComponent<PlayerComponent>();
	playerEntity = nullptr;

	m_playerEntityAddedCheckIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::ECS::OnEntityCreatedEvent>(std::bind(&PlayerSystem::CheckForPlayer, this));
	m_playerMoveUpTransformIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveUpTransformEvent>(std::bind(&PlayerSystem::MovePlayerUpTransform, this));
	m_playerMoveLeftTransformIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveLeftTransformEvent>(std::bind(&PlayerSystem::MovePlayerLeftTransform, this));
	m_playerMoveRightTransformIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveDownTransformEvent>(std::bind(&PlayerSystem::MovePlayerDownTransform, this));
	m_playerMoveDownTransformIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveRightTransformEvent>(std::bind(&PlayerSystem::MovePlayerRightTransform, this));
	m_playerMoveUpIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveUpEvent>(std::bind(&PlayerSystem::MovePlayerUp, this));
	m_playerMoveLeftIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveLeftEvent>(std::bind(&PlayerSystem::MovePlayerLeft, this));
	m_playerMoveRightIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveDownEvent>(std::bind(&PlayerSystem::MovePlayerDown, this));
	m_playerMoveDownIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveRightEvent>(std::bind(&PlayerSystem::MovePlayerRight, this));
	m_interactionEventIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::OnInteractEvent>(std::bind(&PlayerSystem::Interact, this));
	Firelight::Engine::Instance().GetKeyBinder().BindKeyboardActionEvent(Firelight::Events::InputEvents::OnInteractEvent::sm_descriptor, Firelight::Keys::KEY_F);
	m_spawnItemEventIndex = Firelight::Events::EventDispatcher::SubscribeFunction<Firelight::Events::InputEvents::SpawnItemEvent>(std::bind(&PlayerSystem::SpawnItem, this));

	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::InputEvents::OnPlayerMoveEvent>(this);

	imguiLayer = new ImGuiPlayerLayer();
	Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(imguiLayer);
}

PlayerSystem::~PlayerSystem()
{
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::ECS::OnEntityCreatedEvent>(m_playerEntityAddedCheckIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveUpEvent>(m_playerMoveUpIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveLeftEvent>(m_playerMoveLeftIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveDownEvent>(m_playerMoveRightIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveRightEvent>(m_playerMoveDownIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnInteractEvent>(m_interactionEventIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveUpTransformEvent>(m_playerMoveUpTransformIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveLeftTransformEvent>(m_playerMoveLeftTransformIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveDownTransformEvent>(m_playerMoveRightTransformIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::OnPlayerMoveRightTransformEvent>(m_playerMoveDownTransformIndex);
	Firelight::Events::EventDispatcher::UnsubscribeFunction<Firelight::Events::InputEvents::SpawnItemEvent>(m_spawnItemEventIndex);
}

void PlayerSystem::CheckForPlayer()
{
	if (playerEntity == nullptr && m_entities.size() > 0)
	{
		playerEntity = new PlayerEntity(m_entities[0]->GetEntityID());
		imguiLayer->SetPlayer(playerEntity);
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

void PlayerSystem::MovePlayerUpTransform()
{
	playerEntity->GetTransformComponent()->position.y += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerLeftTransform()
{
	playerEntity->GetTransformComponent()->position.x -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerDownTransform()
{
	playerEntity->GetTransformComponent()->position.y -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerRightTransform()
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

void PlayerSystem::SpawnItem()
{
	Entity* itemEntity = ItemDatabase::Instance()->CreateInstanceOfItem(0);
	itemEntity->GetComponent<TransformComponent>()->position = playerEntity->GetTransformComponent()->position;
}

