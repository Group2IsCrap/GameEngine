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
#include "../Items/CraftingRecipeDatabase.h"
#include "../Core/Layers.h"
#include "../Core/CharacterEntity.h"
#include "../Combat/CombatCalculations.h"

#include"../Inventory/InventoryFunctionsGlobal.h"
#include <Source/ECS/Systems/AnimationSystem.h>

using namespace Firelight::Events;
using namespace Firelight::Events::InputEvents;


PlayerSystem::PlayerSystem()
{
	AddWhitelistComponent<PlayerComponent>();
	m_playerEntity = nullptr;

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

	m_updateCraftableItemsEventIndex = EventDispatcher::SubscribeFunction<Inventory::UpdateCraftableItemsEvent>(std::bind(&PlayerSystem::UpdateCraftableItems, this));

	Firelight::Events::EventDispatcher::AddListener<InputEvents::OnPlayerMoveEvent>(this);
	Firelight::Events::EventDispatcher::AddListener<Inventory::LoadInventoryGroup>(this);
	Firelight::Events::EventDispatcher::AddListener<Inventory::UnloadInventoryGroup>(this);

	m_imguiLayer = new ImGuiPlayerLayer();
	Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(m_imguiLayer);
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
	EventDispatcher::UnsubscribeFunction<Inventory::UpdateCraftableItemsEvent>(m_updateCraftableItemsEventIndex);
	
	Firelight::Events::EventDispatcher::RemoveListener<Inventory::LoadInventoryGroup>(this);
	Firelight::Events::EventDispatcher::RemoveListener<Inventory::UnloadInventoryGroup>(this);

	EventDispatcher::UnsubscribeFunction<AttackEvent>(m_attackIndex);
}

void PlayerSystem::CheckForPlayer()
{
	if (m_playerEntity == nullptr && m_entities.size() > 0)
	{
		m_playerEntity = new PlayerEntity(m_entities[0]->GetEntityID());
		m_imguiLayer->SetPlayer(m_playerEntity);
	}
}

void PlayerSystem::Update(const Firelight::Utils::Time& time)
{
	if (m_playerEntity == nullptr)
	{
		return;
	}
	PlayerComponent* playerComponent = m_playerEntity->GetComponent<PlayerComponent>();

	if (m_moveUp)
	{
		playerComponent->facing = Facing::Up;
	}
	else if (m_moveDown)
	{
		playerComponent->facing = Facing::Down;
	}
	else if (m_moveLeft)
	{
		playerComponent->facing = Facing::Left;
	}
	else if (m_moveRight)
	{
		playerComponent->facing = Facing::Right;
	}
	m_attackCooldown += time.GetDeltaTime();
	if (m_attackCooldown >= m_currentWeaponCooldown && m_isAttacking)
	{
		m_attackCooldown = 0.0f;
		Attack();
	}

	HandlePlayerAnimations();
}

void PlayerSystem::FixedUpdate(const Firelight::Utils::Time& time)
{
	if (m_moveUp)
	{
		m_playerEntity->GetRigidBodyComponent()->velocity.y += GetSpeed() * time.GetPhysicsTimeStep();
	}
	if (m_moveDown)
	{
		m_playerEntity->GetRigidBodyComponent()->velocity.y -= GetSpeed() * time.GetPhysicsTimeStep();
	}
	if (m_moveLeft)
	{
		m_playerEntity->GetRigidBodyComponent()->velocity.x -= GetSpeed() * time.GetPhysicsTimeStep();
	}
	if (m_moveRight)
	{
		m_playerEntity->GetRigidBodyComponent()->velocity.x += GetSpeed() * time.GetPhysicsTimeStep();
	}
}

void PlayerSystem::HandleEvents(DescriptorType event, void* data)
{
	if (event == Firelight::Events::InputEvents::OnPlayerMoveEvent::sm_descriptor)
	{
		Firelight::Maths::Vec2f axis = *(reinterpret_cast<Firelight::Maths::Vec2f*>(data));

		m_playerEntity->GetTransformComponent()->position.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_playerEntity->GetComponent<PlayerComponent>()->speed) * axis.x * 2;
		m_playerEntity->GetTransformComponent()->position.y += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_playerEntity->GetComponent<PlayerComponent>()->speed) * axis.y * 2;
	}
	else if (event == Firelight::Events::Inventory::LoadInventoryGroup::sm_descriptor)
	{
		std::string inventoryGroupName = *reinterpret_cast<std::string*>(data);
		
		// When the main player opens their inventory, update what items they can craft
		
		if (inventoryGroupName == "PlayerInventory")
		{
			UpdateCraftableItems();
			m_imguiLayer->SetDebugCraftingMenuEnabled(true);
		}
	}
	else if (event == Firelight::Events::Inventory::UnloadInventoryGroup::sm_descriptor)
	{
		std::string inventoryGroupName = *reinterpret_cast<std::string*>(data);

		if (inventoryGroupName == "PlayerInventory")
		{
			m_imguiLayer->SetDebugCraftingMenuEnabled(false);
		}
	}
}

float PlayerSystem::GetSpeed()
{
	return m_playerEntity->GetComponent<PlayerComponent>()->speed;
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

void PlayerSystem::HandlePlayerAnimations()
{
	if (m_playerEntity == nullptr)
	{
		return;
	}

	if (m_moveRight)
	{
		Firelight::ECS::AnimationSystem::Instance()->Play(m_playerEntity, "PlayerRunRight");
		m_playerEntity->GetSpriteComponent()->flipX = false;
	}
	else if (m_moveLeft)
	{
		// Move left
		Firelight::ECS::AnimationSystem::Instance()->Play(m_playerEntity, "PlayerRunRight");
		m_playerEntity->GetSpriteComponent()->flipX = true;
	}
	else if (m_moveUp || m_moveDown)
	{
		// Prioritize right side when moving
		Firelight::ECS::AnimationSystem::Instance()->Play(m_playerEntity, "PlayerRunRight");
		m_playerEntity->GetSpriteComponent()->flipX = false;
	}
	else if (!m_isAttacking)
	{
		Firelight::ECS::AnimationSystem::Instance()->Play(m_playerEntity, "PlayerIdle");
	}
}

void PlayerSystem::UpdateCraftableItems()
{
	m_availableCraftingRecipes.clear();
	for (const auto* recipe : CraftingRecipeDatabase::Instance().GetAllCraftingRecipes())
	{
		if (recipe->CanCraft("PlayerInventory"))
		{
			m_availableCraftingRecipes.push_back(recipe);
		}
	}
	m_imguiLayer->GiveAvailableCraftingRecipes(&m_availableCraftingRecipes);
}

void PlayerSystem::Interact()
{
	std::vector<Firelight::ECS::Entity*> entitiesCollidedWith = Firelight::Physics::PhysicsHelpers::OverlapCircle(m_playerEntity->GetTransformComponent()->position, 1.0f, static_cast<int>(GameLayer::Items));
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
	}
}

void PlayerSystem::SpawnItem()
{
	Entity* itemEntity = ItemDatabase::Instance()->CreateInstanceOfItem(0);
	itemEntity->GetComponent<TransformComponent>()->position = m_playerEntity->GetTransformComponent()->position;
}

void PlayerSystem::Attack()
{
	Firelight::ECS::AnimationSystem::Instance()->Play(m_playerEntity, "PlayerAttack");
	CombatCalculations::PlaceSphere(m_playerEntity->GetComponent<PlayerComponent>()->facing, m_playerEntity->GetRigidBodyComponent()->nextPos);
}

void PlayerSystem::RemoveHealth()
{
	m_playerEntity->RemoveHealth(1);
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