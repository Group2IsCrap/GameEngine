#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>
#include <Source/Physics/PhysicsHelpers.h>
#include <Source/ImGuiUI/ImGuiManager.h>
#include<Source/ECS/Components/ItemComponents.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"
#include "../Items/ItemDatabase.h"
#include "../Core/Layers.h"
#include "../Core/CharacterEntity.h"
#include "../Combat/CombatCalculations.h"

#include"../Inventory/InventoryFunctionsGlobal.h"
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

	m_playerMoveUpReleaseIndex = EventDispatcher::SubscribeFunction<OnPlayerMoveUpEventRelease>(std::bind(&PlayerSystem::MovePlayerUpRelease, this));
	m_playerMoveLeftReleaseIndex = EventDispatcher::SubscribeFunction<OnPlayerMoveLeftEventRelease>(std::bind(&PlayerSystem::MovePlayerLeftRelease, this));
	m_playerMoveRightReleaseIndex = EventDispatcher::SubscribeFunction<OnPlayerMoveDownEventRelease>(std::bind(&PlayerSystem::MovePlayerDownRelease, this));
	m_playerMoveDownReleaseIndex = EventDispatcher::SubscribeFunction<OnPlayerMoveRightEventRelease>(std::bind(&PlayerSystem::MovePlayerRightRelease, this));
	
	m_interactionEventIndex = EventDispatcher::SubscribeFunction<OnInteractEvent>(std::bind(&PlayerSystem::Interact, this));
	m_spawnItemEventIndex = EventDispatcher::SubscribeFunction<SpawnItemEvent>(std::bind(&PlayerSystem::SpawnItem, this));
	m_removeHealthEventIndex = EventDispatcher::SubscribeFunction<RemoveHealthEvent>(std::bind(&PlayerSystem::RemoveHealth, this));
	m_attackIndex = EventDispatcher::SubscribeFunction<AttackEvent>(std::bind(&PlayerSystem::StartAttack, this));
	m_releaseAttackIndex = EventDispatcher::SubscribeFunction<ReleaseAttackEvent>(std::bind(&PlayerSystem::StopAttack, this));

	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::InputEvents::OnPlayerMoveEvent>(this);

	imguiLayer = new ImGuiPlayerLayer();
	Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(imguiLayer);
}

PlayerSystem::~PlayerSystem()
{
	EventDispatcher::UnsubscribeFunction<ECS::OnEntityCreatedEvent>(m_playerEntityAddedCheckIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveUpEvent>(m_playerMoveUpIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveLeftEvent>(m_playerMoveLeftIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveDownEvent>(m_playerMoveRightIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveRightEvent>(m_playerMoveDownIndex);

	EventDispatcher::UnsubscribeFunction<OnPlayerMoveUpEvent>(m_playerMoveUpReleaseIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveLeftEvent>(m_playerMoveLeftReleaseIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveDownEvent>(m_playerMoveRightReleaseIndex);
	EventDispatcher::UnsubscribeFunction<OnPlayerMoveRightEvent>(m_playerMoveDownReleaseIndex);

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
		imguiLayer->SetPlayer(playerEntity);
	}
}

void PlayerSystem::Update(const Firelight::Utils::Time& time)
{
	if (playerEntity == nullptr)
	{
		return;
	}
	if (m_moveUp)
	{
		playerEntity->GetComponent<PlayerComponent>()->facing = Facing::Up;
	}
	else if (m_moveDown)
	{
		playerEntity->GetComponent<PlayerComponent>()->facing = Facing::Down;
	}
	else if (m_moveLeft)
	{
		playerEntity->GetComponent<PlayerComponent>()->facing = Facing::Left;
	}
	else if (m_moveRight)
	{
		playerEntity->GetComponent<PlayerComponent>()->facing = Facing::Right;
	}
	m_attackCooldown += time.GetDeltaTime();
	if (m_attackCooldown >= m_currentWeaponCooldown && m_isAttacking)
	{
		m_attackCooldown = 0.0f;
		Attack();
	}
}

void PlayerSystem::FixedUpdate(const Firelight::Utils::Time& time)
{
	if (m_moveUp)
	{
		playerEntity->GetRigidBodyComponent()->velocity.y += GetSpeed() * time.GetPhysicsTimeStep();
	}
	if (m_moveDown)
	{
		playerEntity->GetRigidBodyComponent()->velocity.y -= GetSpeed() * time.GetPhysicsTimeStep();
	}
	if (m_moveLeft)
	{
		playerEntity->GetRigidBodyComponent()->velocity.x -= GetSpeed() * time.GetPhysicsTimeStep();
	}
	if (m_moveRight)
	{
		playerEntity->GetRigidBodyComponent()->velocity.x += GetSpeed() * time.GetPhysicsTimeStep();
	}
}

void PlayerSystem::HandleEvents(DescriptorType event, void* data)
{
	if (event == Firelight::Events::InputEvents::OnPlayerMoveEvent::sm_descriptor)
	{
		Firelight::Maths::Vec2f axis = *(reinterpret_cast<Firelight::Maths::Vec2f*>(data));
	}
}

float PlayerSystem::GetSpeed()
{
	return playerEntity->GetComponent<PlayerComponent>()->speed;
}

void PlayerSystem::MovePlayerUp()
{
	m_moveUp = true;
}
void PlayerSystem::MovePlayerLeft()
{
	m_moveLeft = true;
}
void PlayerSystem::MovePlayerDown()
{
	m_moveDown = true;
}
void PlayerSystem::MovePlayerRight()
{
	m_moveRight = true;
}

void PlayerSystem::MovePlayerUpRelease()
{
	m_moveUp = false;
}
void PlayerSystem::MovePlayerLeftRelease()
{
	m_moveLeft = false;
}
void PlayerSystem::MovePlayerDownRelease()
{
	m_moveDown = false;
}
void PlayerSystem::MovePlayerRightRelease()
{
	m_moveRight = false;
}

void PlayerSystem::Interact()
{
	std::vector<Firelight::ECS::Entity*> entitiesCollidedWith = Firelight::Physics::PhysicsHelpers::OverlapCircle(playerEntity->GetTransformComponent()->position, 1.0f, static_cast<int>(GameLayer::Items));
	if (entitiesCollidedWith.size() > 0)
	{
		TransformComponent* transformComponent = entitiesCollidedWith[0]->GetComponent<TransformComponent>();
		if (entitiesCollidedWith[0]->HasComponent<AudioComponent>())
		{
			AudioComponent* audioComponent = entitiesCollidedWith[0]->GetComponent<AudioComponent>();
			
			audioComponent->soundPos = { transformComponent->position.x,  transformComponent->position.y,  transformComponent->position.z };
			entitiesCollidedWith[0]->PlayAudioClip();
		}
		//ckeck if it is a item
		if (entitiesCollidedWith[0]->HasComponent<Firelight::ECS::ItemComponent>()) {

			if (!InventorySystem::GlobalFunctions::AddItem("PlayerInventory", "MainIven", entitiesCollidedWith[0])) {
				//hide item
				transformComponent->position = Vec3f(100000, 0, 0);
			}
		}
		//playerEntity->GetTransformComponent()->position.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
	}
}

void PlayerSystem::SpawnItem()
{
	Entity* itemEntity = ItemDatabase::Instance()->CreateInstanceOfItem(0);
	itemEntity->GetComponent<TransformComponent>()->position = playerEntity->GetTransformComponent()->position;
}

void PlayerSystem::Attack()
{
	CombatCalculations::PlaceSphere(playerEntity->GetComponent<PlayerComponent>()->facing, playerEntity->GetRigidBodyComponent()->nextPos);
}

void PlayerSystem::RemoveHealth()
{
	playerEntity->RemoveHealth(1);
}

void PlayerSystem::SwitchWeapon()
{
	//Get current weapon from equipped & cooldown
	//Swap currentWeaponCooldown
}

void PlayerSystem::StartAttack()
{
	m_isAttacking = true;
}

void PlayerSystem::StopAttack()
{
	m_isAttacking = false;
}