#include "PlayerEntity.h"

#include <Source/Graphics/AssetManager.h>
#include <Source/Events/EventDispatcher.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include <Source/ECS/Components/ItemComponents.h>
#include "../Events/PlayerEvents.h"
#include "../Core/Layers.h"

#include"../Inventory/InventoryFunctionsGlobal.h"

#include "PlayerComponent.h"
#include <Source/ECS/Components/AnimationComponent.h>

PlayerEntity::PlayerEntity()
{
	GetComponent<Firelight::ECS::IdentificationComponent>()->name = "Player";
	GetComponent<Firelight::ECS::SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PlayerIdle.png");
	AddComponent<PlayerComponent>()->speed;
	GetComponent<Firelight::ECS::RigidBodyComponent>()->interpolate = true;
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Player);
	GetLayerComponent()->layer = static_cast<int>(GameLayer::Player);
	GetSpriteComponent()->pixelsPerUnit *= 2;
	AddComponent<Firelight::ECS::AnimationComponent>();

	GetHealthComponent()->maxHealth = 20;
	GetHealthComponent()->currentHealth = GetHealthComponent()->maxHealth;

	GameEntity* weaponSocket = new GameEntity();
	weaponSocket->GetTransformComponent()->SetPosition({ 0.65f, -0.45f, 0.0f });
	GetTransformComponent()->AddChild(weaponSocket);
	GetComponent<PlayerComponent>()->weaponSocket = weaponSocket;

	GameEntity* hatSocket = new GameEntity();
	hatSocket->GetTransformComponent()->SetPosition({ 0.65f, -0.45f, 0.0f });
	GetTransformComponent()->AddChild(hatSocket);
	GetComponent<PlayerComponent>()->hatSocket = hatSocket;

	SpriteEntity* hat = new SpriteEntity();
	hat->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/Items/Armor/LeatherHat.png");
	hat->GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Player) + 1;
	hat->GetSpriteComponent()->pixelsPerUnit *= 3;
	hat->GetTransformComponent()->SetPosition({ 0.0f, 1.0f, 0.0f });
	GetTransformComponent()->AddChild(hat);

	AudioComponent* audioComponent = new AudioComponent();
	AddComponent<Firelight::ECS::AudioComponent>(audioComponent);
	audioComponent->looping = false;
	audioComponent->is3d = false;
	audioComponent->streaming = false;
	audioComponent->channel = "Player";

	Firelight::ECS::BoxColliderComponent* boxCollider = dynamic_cast<Firelight::ECS::BoxColliderComponent*>(AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::BoxColliderComponent()));
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 1.0f, 2.0f);
}

PlayerEntity::PlayerEntity(Firelight::ECS::EntityID entityID) : CharacterEntity(entityID)
{
	
}

void PlayerEntity::HealthBelowZero()
{
	Firelight::Events::EventDispatcher::InvokeListeners<Firelight::Events::PlayerEvents::OnPlayerDiedEvent>(this);
}

void PlayerEntity::RemoveHealth(int amount)
{
	int reduction = 0;
	EntityID id = InventorySystem::GlobalFunctions::GetSpecialSlotEntity("PlayerInventory", "Equipment", "Head");
	EntityID id2 = InventorySystem::GlobalFunctions::GetSpecialSlotEntity("PlayerInventory", "Equipment", "Body");

	ArmourComponent* armour = nullptr;

	if (id != UINT16_MAX)
	{
		Entity activeHat = Entity(id);
		armour= activeHat.GetComponent<ArmourComponent>();
		if (armour != nullptr)
		{
			reduction += armour->armourValue;
		}
	}
	if (id2 != UINT16_MAX)
	{
		Entity activeChest = Entity(id2);
		armour = activeChest.GetComponent<ArmourComponent>();
		if (armour != nullptr)
		{
			reduction += armour->armourValue;
		}
	}

	AudioComponent* audioComponent = this->GetComponent<AudioComponent>();

	switch (Firelight::Maths::Random::RandomRange(0, 1))
	{
	case 0:
		audioComponent->soundName = "Hurt 1.wav";
		break;
	case 1:
		audioComponent->soundName = "Hurt 2.wav";
		break;
	}

	audioComponent->soundPos = Vector3D(this->GetTransformComponent()->GetPosition().x, this->GetTransformComponent()->GetPosition().y, this->GetTransformComponent()->GetPosition().z);

	int damage = 0;

	this->PlayAudioClip();
	if (reduction >= amount)
	{
		damage = 1;
	}
	else
	{
		damage = amount - reduction;
	}
	CharacterEntity::RemoveHealth(amount-reduction);
	Firelight::Events::EventDispatcher::InvokeListeners<Firelight::Events::PlayerEvents::OnPlayerHealthChangedEvent>((void*)GetHealth());
}

void PlayerEntity::AddHealth(int amount)
{
	CharacterEntity::AddHealth(amount);
	Firelight::Events::EventDispatcher::InvokeListeners<Firelight::Events::PlayerEvents::OnPlayerHealthChangedEvent>((void*)GetHealth());
}
void PlayerEntity::PlayerHealthUpdated()
{

}
