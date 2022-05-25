#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>
#include <Source/Physics/PhysicsHelpers.h>s
#include <Source/ImGuiUI/ImGuiManager.h>
#include <Source/Graphics/Data/Colour.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"
#include "../Items/ItemDatabase.h"
#include "../Items/CraftingRecipeDatabase.h"
#include "../Core/Layers.h"
#include "../Core/CharacterEntity.h"
#include "../Combat/CombatCalculations.h"

#include"../Inventory/InventoryFunctionsGlobal.h"

#include <Source/ECS/Systems/AnimationSystem.h>

#include "../PCG/BiomeGeneration.h"
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
	m_changeHatIndex = EventDispatcher::SubscribeFunction<Firelight::Events::PlayerEvents::ChangeHat>(std::bind(&PlayerSystem::SwitchHat, this));
	m_changeArmourIndex = EventDispatcher::SubscribeFunction<Firelight::Events::PlayerEvents::ChangeArmour>(std::bind(&PlayerSystem::SwitchArmour, this));

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
			m_playerEntity->GetRigidBodyComponent()->velocity.y += GetSpeed() * static_cast<float>(time.GetPhysicsTimeStep());
		}
		if (m_moveDown)
		{
			m_playerEntity->GetRigidBodyComponent()->velocity.y -= GetSpeed() * static_cast<float>(time.GetPhysicsTimeStep());
		}
		if (m_moveLeft)
		{
			m_playerEntity->GetRigidBodyComponent()->velocity.x -= GetSpeed() * static_cast<float>(time.GetPhysicsTimeStep());
		}
		if (m_moveRight)
		{
			m_playerEntity->GetRigidBodyComponent()->velocity.x += GetSpeed() * static_cast<float>(time.GetPhysicsTimeStep());
		}

		if (BiomeGeneration::Instance()->IsInVoid(Rectf(m_playerEntity->GetTransformComponent()->GetPosition().x, m_playerEntity->GetTransformComponent()->GetPosition().y, 1.0f, 1.0f)))
		{
			//m_playerEntity->GetRigidBodyComponent()->velocity.x = -m_playerEntity->GetRigidBodyComponent()->velocity.x + 0.2;
			//m_playerEntity->GetRigidBodyComponent()->velocity.y = -m_playerEntity->GetRigidBodyComponent()->velocity.y +0.2;
		}
		BiomeGeneration::Instance()->CheckCurrentPlayerBiomeType(Rectf(m_playerEntity->GetTransformComponent()->GetPosition().x, m_playerEntity->GetTransformComponent()->GetPosition().y, 1.0f, 1.0f));
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
	

	CombatCalculations::PlaceSphere(m_playerEntity->GetComponent<PlayerComponent>()->facing, m_playerEntity->GetRigidBodyComponent()->nextPos, m_playerEntity);
}

void PlayerSystem::RemoveHealth()
{
	

	m_playerEntity->RemoveHealth(1);
}

void PlayerSystem::AddHealth(void* amount)
{
	AudioComponent* audioComponent = m_playerEntity->GetComponent<AudioComponent>();

	audioComponent->soundName = "Cheerful Character 1.wav";

	audioComponent->soundPos = Vector3D(m_playerEntity->GetTransformComponent()->GetPosition().x, m_playerEntity->GetTransformComponent()->GetPosition().y, m_playerEntity->GetTransformComponent()->GetPosition().z);

	m_playerEntity->PlayAudioClip();

	int amountAdd = (int)amount;
	m_playerEntity->AddHealth(amountAdd);
}

void PlayerSystem::SwitchHat()
{
	PlayerComponent* playerComponent = m_playerEntity->GetComponent<PlayerComponent>();

	EntityID id = InventorySystem::GlobalFunctions::GetSpecialSlotEntity("PlayerInventory", "Equipment", "Head");
	ArmourComponent* armourComponent = nullptr;
	//EntityID id2 = InventorySystem::GlobalFunctions::GetSpecialSlotEntity("PlayerInventory", "Equipment", "Body");

	ArmourComponent* armour = nullptr;

	if (id != UINT16_MAX)
	{
		Entity activeHat = Entity(id);
		armour = activeHat.GetComponent<ArmourComponent>();
		if (playerComponent->hat == nullptr)
		{
			SpriteEntity* hat = new SpriteEntity();
			playerComponent->hat = hat;
			playerComponent->hatSocket->GetComponent<TransformComponent>()->AddChild(playerComponent->hat);
		}

		SpriteComponent* hatSpriteComponent = playerComponent->hat->GetComponent<SpriteComponent>();
		hatSpriteComponent->texture = activeHat.GetComponent<SpriteComponent>()->texture;
		hatSpriteComponent->layer = static_cast<int>(RenderLayer::Player) + 1;
		hatSpriteComponent->pixelsPerUnit = activeHat.GetComponent<SpriteComponent>()->pixelsPerUnit;
		hatSpriteComponent->colour = Firelight::Graphics::Colour::RGBA(255, 255, 255, 255);

		playerComponent->hat->GetComponent<TransformComponent>()->SetPosition(playerComponent->hatSocket->GetComponent<TransformComponent>()->GetPosition());
		playerComponent->hat->GetComponent<TransformComponent>()->FlipX(playerComponent->hatSocket->GetComponent<TransformComponent>()->GetFlipped(), false);
	}
}

void PlayerSystem::SwitchArmour()
{
	PlayerComponent* playerComponent = m_playerEntity->GetComponent<PlayerComponent>();

	EntityID id = InventorySystem::GlobalFunctions::GetSpecialSlotEntity("PlayerInventory", "Equipment", "Body");
	ArmourComponent* armourComponent = nullptr;

	ArmourComponent* armour = nullptr;

	if (id != UINT16_MAX)
	{
		Entity activeArmour = Entity(id);
		armour = activeArmour.GetComponent<ArmourComponent>();
		if (playerComponent->body == nullptr)
		{
			SpriteEntity* body = new SpriteEntity();
			playerComponent->body = body;
			playerComponent->bodySocket->GetComponent<TransformComponent>()->AddChild(playerComponent->body);
		}

		SpriteComponent* bodySpriteComponent = playerComponent->body->GetComponent<SpriteComponent>();
		bodySpriteComponent->texture = activeArmour.GetComponent<SpriteComponent>()->texture;
		bodySpriteComponent->layer = static_cast<int>(RenderLayer::Player) + 1;
		bodySpriteComponent->pixelsPerUnit = activeArmour.GetComponent<SpriteComponent>()->pixelsPerUnit;
		bodySpriteComponent->colour = Firelight::Graphics::Colour::RGBA(255, 255, 255, 255);

		playerComponent->body->GetComponent<TransformComponent>()->SetPosition(playerComponent->bodySocket->GetComponent<TransformComponent>()->GetPosition());
		playerComponent->body->GetComponent<TransformComponent>()->FlipX(playerComponent->bodySocket->GetComponent<TransformComponent>()->GetFlipped(), false);
	}
}

void PlayerSystem::SwitchWeapon()
{
	EntityID id = InventorySystem::GlobalFunctions::GetSpecialSlotEntity("PlayerInventory", "Equipment", "Weapon");
 	WeaponComponent* weaponComponent = nullptr;
	PlayerComponent* playerComponent = m_playerEntity->GetComponent<PlayerComponent>();

	if (id != UINT16_MAX)
	{
		Entity activeWeapon = Entity(id);
		weaponComponent = activeWeapon.GetComponent<WeaponComponent>();
		if (activeWeapon.HasComponent<SpriteComponent>())
		{
			if (playerComponent->weapon == nullptr)
			{
				SpriteEntity* weapon = new SpriteEntity();
				playerComponent->weapon = weapon;
				playerComponent->weaponSocket->GetComponent<TransformComponent>()->AddChild(playerComponent->weapon);
			}

			SpriteComponent* weaponSpriteComponent = playerComponent->weapon->GetComponent<SpriteComponent>();
			weaponSpriteComponent->texture = activeWeapon.GetComponent<SpriteComponent>()->texture;
			weaponSpriteComponent->layer = static_cast<int>(RenderLayer::Player) + 2;
			weaponSpriteComponent->pixelsPerUnit = activeWeapon.GetComponent<SpriteComponent>()->pixelsPerUnit;
			weaponSpriteComponent->colour = Firelight::Graphics::Colour::RGBA(255, 255, 255, 255);

			playerComponent->weapon->GetComponent<TransformComponent>()->SetPosition(playerComponent->weaponSocket->GetComponent<TransformComponent>()->GetPosition());
			playerComponent->weapon->GetComponent<TransformComponent>()->FlipX(playerComponent->weaponSocket->GetComponent<TransformComponent>()->GetFlipped(), false);
		}
	}
	else
	{
		weaponComponent = fists;
		if (playerComponent->weapon != nullptr)
		{
			playerComponent->weapon->GetComponent<SpriteComponent>()->colour = Firelight::Graphics::Colour::RGBA(0, 0, 0, 0);
		}
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

		AudioComponent* audioComponent = m_playerEntity->GetComponent<AudioComponent>();

		audioComponent->soundName = "Respawn.wav";

		audioComponent->soundPos = Vector3D(m_playerEntity->GetTransformComponent()->GetPosition().x, m_playerEntity->GetTransformComponent()->GetPosition().y, m_playerEntity->GetTransformComponent()->GetPosition().z);

		m_playerEntity->PlayAudioClip();

		InventorySystem::GlobalFunctions::RemoveAllItems("PlayerInventory", "MainInventory");
		InventorySystem::GlobalFunctions::RemoveAllItems("PlayerInventory", "Equipment");

		m_playerEntity->AddHealth(m_playerEntity->GetHealthComponent()->maxHealth);

		m_playerEntity->GetRigidBodyComponent()->nextPos = Vec3f(0.0f,0.0f,0.0f);

	}
}