#include "PlayerEntity.h"

#include <Source/Graphics/AssetManager.h>
#include <Source/Events/EventDispatcher.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include "../Events/PlayerEvents.h"
#include "../Core/Layers.h"

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

	GetHealthComponent()->maxHealth = 5;
	GetHealthComponent()->currentHealth = GetHealthComponent()->maxHealth;

	GameEntity* weaponSocket = new GameEntity();
	weaponSocket->GetTransformComponent()->SetPosition({ 0.65f, -0.45f, 0.0f });
	GetTransformComponent()->AddChild(weaponSocket);
	GetComponent<PlayerComponent>()->weaponSocket = weaponSocket;

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
	CharacterEntity::RemoveHealth(amount);
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
