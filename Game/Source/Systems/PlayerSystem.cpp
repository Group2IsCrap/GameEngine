#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>
#include <Source/Physics/PhysicsHelpers.h>
#include <Source/ImGuiUI/ImGuiManager.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"
#include "../Items/ItemDatabase.h"
#include "../Items/CraftingRecipeDatabase.h"
#include "../Core/Layers.h"
#include "../Core/CharacterEntity.h"
#include "../Combat/CombatCalculations.h"

#include"../Inventory/InventoryFunctionsGlobal.h"

#include <Source/ECS/Systems/AnimationSystem.h>

#include"../Events/PlayerEvents.h"

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
	m_addHealthEventIndex = EventDispatcher::SubscribeFunction<Firelight::Events::PlayerEvents::AddHealth>(std::bind(&PlayerSystem::AddHealth, this, std::placeholders::_1));

	m_attackIndex = EventDispatcher::SubscribeFunction<AttackEvent>(std::bind(&PlayerSystem::StartAttack, this));
	m_releaseAttackIndex = EventDispatcher::SubscribeFunction<ReleaseAttackEvent>(std::bind(&PlayerSystem::StopAttack, this));
	m_respawnIndex = EventDispatcher::SubscribeFunction<RespawnEvent>(std::bind(&PlayerSystem::Respawn, this));
	m_changeWeaponIndex = EventDispatcher::SubscribeFunction<Firelight::Events::PlayerEvents::ChangeWeapon>(std::bind(&PlayerSystem::SwitchWeapon, this));

	Firelight::Events::EventDispatcher::SubscribeFunction<ShowDebugEvent>(std::bind(&PlayerSystem::ToggleDebug, this));
	

	m_updateCraftableItemsEventIndex = EventDispatcher::SubscribeFunction<Inventory::UpdateCraftableItemsEvent>(std::bind(&PlayerSystem::UpdateCraftableItems, this));

	Firelight::Events::EventDispatcher::AddListener<InputEvents::OnPlayerMoveEvent>(this);
	Firelight::Events::EventDispatcher::AddListener<Inventory::LoadInventoryGroup>(this);
	Firelight::Events::EventDispatcher::AddListener<Inventory::UnloadInventoryGroup>(this);

	m_imguiLayer = new ImGuiPlayerLayer();

	fists = new WeaponComponent;
	fists->Radius = 2.0f;
	fists->Angle = 40.0f;
	fists->Damage = 1.0f;
	fists->Cooldown = 1.0f;
	fists->HarvestType = Firelight::ECS::e_HarvestType(0);
	fists->HarvestDamage = 1.0f;

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

	EventDispatcher::UnsubscribeFunction<RemoveHealthEvent>(m_removeHealthEventIndex);
	EventDispatcher::UnsubscribeFunction<AttackEvent>(m_attackIndex);
	EventDispatcher::UnsubscribeFunction<ReleaseAttackEvent>(m_releaseAttackIndex);
	EventDispatcher::UnsubscribeFunction<RespawnEvent>(m_respawnIndex);
	EventDispatcher::UnsubscribeFunction<Firelight::Events::PlayerEvents::ChangeWeapon>(m_changeWeaponIndex);
}

void PlayerSystem::CheckForPlayer()
{
	if (m_playerEntity == nullptr && m_entities.size() > 0)
	{
		m_playerEntity = new PlayerEntity(m_entities[0]->GetEntityID());
		m_imguiLayer->SetPlayer(m_playerEntity);
	}
}

void PlayerSystem::Update(const Firelight::Utils::Time& time, const bool& isPaused)
{
	if (!isPaused)
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
}

void PlayerSystem::FixedUpdate(const Firelight::Utils::Time& time, const bool& isPaused)
{
	if (!isPaused)
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
}

void PlayerSystem::HandleEvents(DescriptorType event, void* data)
{
	if (event == Firelight::Events::InputEvents::OnPlayerMoveEvent::sm_descriptor)
	{
		Firelight::Maths::Vec2f axis = *(reinterpret_cast<Firelight::Maths::Vec2f*>(data));

		if (auto* transformComponent = m_playerEntity->GetTransformComponent())
		{
			Firelight::Maths::Vec3f pos = transformComponent->GetPosition();
			float xDiff = static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_playerEntity->GetComponent<PlayerComponent>()->speed) * axis.x * 2;
			float yDiff = static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_playerEntity->GetComponent<PlayerComponent>()->speed) * axis.y * 2;
			transformComponent->SetPosition(pos + Firelight::Maths::Vec3f(xDiff, yDiff, 0.0f));
		}
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
	if (!Firelight::Engine::Instance().GetPaused())
	{
		if (m_playerEntity != nullptr)
		{
			m_playerEntity->GetTransformComponent()->FlipX(true);
		}
		m_moveLeft = true;
	}
}
void PlayerSystem::MovePlayerDown()
{
	m_moveDown = true;
}
void PlayerSystem::MovePlayerRight()
{
	if (!Firelight::Engine::Instance().GetPaused())
	{
		if (m_playerEntity != nullptr)
		{
			m_playerEntity->GetTransformComponent()->FlipX(false);
		}
		m_moveRight = true;
	}
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
	std::vector<Firelight::ECS::Entity*> entitiesCollidedWith = Firelight::Physics::PhysicsHelpers::OverlapCircle(m_playerEntity->GetTransformComponent()->GetPosition(), 1.0f, static_cast<int>(GameLayer::Items));

	if (entitiesCollidedWith.size() > 0)
	{
		TransformComponent* transformComponent = entitiesCollidedWith[0]->GetComponent<TransformComponent>();
		if (entitiesCollidedWith[0]->HasComponent<AudioComponent>())
		{
			AudioComponent* audioComponent = entitiesCollidedWith[0]->GetComponent<AudioComponent>();
			
			audioComponent->soundPos = { transformComponent->GetPosition().x,  transformComponent->GetPosition().y,  transformComponent->GetPosition().z};
			entitiesCollidedWith[0]->PlayAudioClip();
		}
		//ckeck if it is a item
		if (entitiesCollidedWith[0]->HasComponent<Firelight::ECS::ItemComponent>()) {

			if (!InventorySystem::GlobalFunctions::AddItem("PlayerInventory", "MainInventory", entitiesCollidedWith[0])) {
				//hide item
				transformComponent->SetPosition(Vec3f(100000, 0, 0));
			}
		}
	}
}

void PlayerSystem::SpawnItem()
{
	Entity* itemEntity = ItemDatabase::Instance()->CreateInstanceOfItem(0);

	itemEntity->GetComponent<TransformComponent>()->SetPosition(m_playerEntity->GetTransformComponent()->GetPosition());
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
void PlayerSystem::AddHealth(void* amount)
{
	int amountAdd = (int)amount;
	m_playerEntity->AddHealth(amountAdd);
}
void PlayerSystem::SwitchWeapon()
{
	EntityID id = InventorySystem::GlobalFunctions::GetSpecialSlotEntity("PlayerInventory", "Equipment", "Weapon");
 	WeaponComponent* weaponComponent = nullptr;
	if (id != UINT16_MAX)
	{
		Entity activeWeapon = Entity(id);
		weaponComponent = activeWeapon.GetComponent<WeaponComponent>();
		if (activeWeapon.HasComponent<SpriteComponent>())
		{
			PlayerComponent* playerComponent = m_playerEntity->GetComponent<PlayerComponent>();
			if (playerComponent->weapon == nullptr)
			{
				SpriteEntity* weapon = new SpriteEntity();
				playerComponent->weapon = weapon;
			}
			if (playerComponent->weapon != nullptr)
			{
				playerComponent->weaponSocket->GetComponent<TransformComponent>()->RemoveChild(playerComponent->weapon);
			}

			playerComponent->weapon->GetComponent<SpriteComponent>()->texture = activeWeapon.GetComponent<SpriteComponent>()->texture;
			playerComponent->weapon->GetComponent<SpriteComponent>()->pixelsPerUnit = activeWeapon.GetComponent<SpriteComponent>()->pixelsPerUnit;
			playerComponent->weapon->GetComponent<TransformComponent>()->SetPosition(playerComponent->weaponSocket->GetComponent<TransformComponent>()->GetPosition());
			playerComponent->weaponSocket->GetComponent<TransformComponent>()->AddChild(playerComponent->weapon);
		}
	}
	else
	{
		weaponComponent = fists;
	}

	if (weaponComponent != nullptr)
	{
		m_currentWeaponCooldown = weaponComponent->Cooldown;

		CombatCalculations::ChangeWeapon(weaponComponent);
		
	}
}

void PlayerSystem::StartAttack()
{
	m_isAttacking = true;
}

void PlayerSystem::StopAttack()
{
	m_isAttacking = false;
}

void PlayerSystem::ToggleDebug()
{
	m_drawDebugUI = !m_drawDebugUI;

	if (m_drawDebugUI)
	{
		Firelight::ImGuiUI::ImGuiManager::Instance()->AddRenderLayer(m_imguiLayer);
	}
	else
	{
		Firelight::ImGuiUI::ImGuiManager::Instance()->RemoveRenderLayer(m_imguiLayer);
	}
}

void PlayerSystem::Respawn()
{
	if (m_playerEntity != nullptr)
	{
		InventorySystem::GlobalFunctions::RemoveAllItems("PlayerInventory", "MainInventory");
		InventorySystem::GlobalFunctions::RemoveAllItems("PlayerInventory", "Equipment");

		m_playerEntity->AddHealth(m_playerEntity->GetHealthComponent()->maxHealth);

		m_playerEntity->GetRigidBodyComponent()->nextPos = Vec3f(0.0f,0.0f,0.0f);

	}
}