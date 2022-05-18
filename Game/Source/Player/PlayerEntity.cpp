#include "PlayerEntity.h"

#include <Source/Graphics/AssetManager.h>
#include <Source/Events/EventDispatcher.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include "../Events/PlayerEvents.h"
#include "../Core/Layers.h"

#include "PlayerComponent.h"

PlayerEntity::PlayerEntity()
{
	GetComponent<Firelight::ECS::IdentificationComponent>()->name = "Player";
	GetComponent<Firelight::ECS::SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PlayerIdle.png");
	AddComponent<PlayerComponent>();
	GetComponent<Firelight::ECS::RigidBodyComponent>()->interpolate = true;
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Player);
	GetLayerComponent()->layer = static_cast<int>(GameLayer::Player);

	GetHealthComponent()->maxHealth = 5;
	GetHealthComponent()->currentHealth = GetHealthComponent()->maxHealth;

	Firelight::ECS::BoxColliderComponent* boxCollider = dynamic_cast<Firelight::ECS::BoxColliderComponent*>(AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::BoxColliderComponent()));
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 1.0f, 2.0f);
	boxCollider->drawCollider = true;
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

void PlayerEntity::PlayerHealthUpdated()
{

}
