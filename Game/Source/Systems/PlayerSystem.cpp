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

#include"../Inventory/InventoryFunctionsGlobal.h"

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
	m_interactionEventIndex = EventDispatcher::SubscribeFunction<OnInteractEvent>(std::bind(&PlayerSystem::Interact, this));
	m_spawnItemEventIndex = EventDispatcher::SubscribeFunction<SpawnItemEvent>(std::bind(&PlayerSystem::SpawnItem, this));
	m_removeHealthEventIndex = EventDispatcher::SubscribeFunction<RemoveHealthEvent>(std::bind(&PlayerSystem::RemoveHealth, this));

	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::InputEvents::OnPlayerMoveEvent>(this);
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::Inventory::LoadInventoryGroup>(this);

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
	EventDispatcher::UnsubscribeFunction<OnInteractEvent>(m_interactionEventIndex);
	EventDispatcher::UnsubscribeFunction<SpawnItemEvent>(m_spawnItemEventIndex);
	EventDispatcher::UnsubscribeFunction<RemoveHealthEvent>(m_removeHealthEventIndex);

	Firelight::Events::EventDispatcher::RemoveListener<Firelight::Events::Inventory::LoadInventoryGroup>(this);
	Firelight::Events::EventDispatcher::RemoveListener<Firelight::Events::Inventory::UnloadInventoryGroup>(this);
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
		m_availableCraftingRecipes.clear();

		if (inventoryGroupName == "PlayerInventory")
		{
			for (const auto* recipe : CraftingRecipeDatabase::Instance().GetAllCraftingRecipes())
			{
				if (recipe->CanCraft(inventoryGroupName))
				{
					m_availableCraftingRecipes.push_back(recipe);
				}
			}

			m_imguiLayer->SetDebugCraftingMenuEnabled(true);
			m_imguiLayer->GiveAvailableCraftingRecipes(&m_availableCraftingRecipes);
		}
	}
}

void PlayerSystem::MovePlayerUp()
{
	m_playerEntity->GetRigidBodyComponent()->velocity.y += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_playerEntity->GetComponent<PlayerComponent>()->speed);
}
void PlayerSystem::MovePlayerLeft()
{
	m_playerEntity->GetRigidBodyComponent()->velocity.x -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_playerEntity->GetComponent<PlayerComponent>()->speed);

}
void PlayerSystem::MovePlayerDown()
{
	m_playerEntity->GetRigidBodyComponent()->velocity.y -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_playerEntity->GetComponent<PlayerComponent>()->speed);

}
void PlayerSystem::MovePlayerRight()
{
	m_playerEntity->GetRigidBodyComponent()->velocity.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_playerEntity->GetComponent<PlayerComponent>()->speed);

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
		//playerEntity->GetTransformComponent()->position.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * playerEntity->GetComponent<PlayerComponent>()->speed);
	}
}

void PlayerSystem::SpawnItem()
{
	Entity* itemEntity = ItemDatabase::Instance()->CreateInstanceOfItem(0);
	itemEntity->GetComponent<TransformComponent>()->position = m_playerEntity->GetTransformComponent()->position;
}

void PlayerSystem::RemoveHealth()
{
	m_playerEntity->RemoveHealth(1);
}

