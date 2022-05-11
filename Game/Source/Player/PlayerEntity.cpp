#include "PlayerEntity.h"

#include <Source/Graphics/AssetManager.h>
#include <Source/Events/EventDispatcher.h>
#include <Source/ECS/Components/PhysicsComponents.h>

#include "PlayerComponent.h"

PlayerEntity::PlayerEntity()
{
	GetComponent<Firelight::ECS::SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PlayerIdle.png");
	AddComponent<PlayerComponent>();
	AddComponent<Firelight::ECS::RigidBodyComponent>()->interpolate = true;
	AddComponent<Firelight::ECS::ColliderComponent>();
	GetSpriteComponent()->layer = 60;

	AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::CircleColliderComponent());
	GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>()->drawCollider = true;
	GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::CircleColliderComponent>()->radius = 1.0f;
}

PlayerEntity::PlayerEntity(Firelight::ECS::EntityID entityID) : CharacterEntity(entityID)
{
	
}

void PlayerEntity::HealthBelowZero()
{
	Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::PlayerDied>();
}

void PlayerEntity::RemoveHealth(int amount)
{
	CharacterEntity::RemoveHealth(amount);
	int health = GetHealth();
	Firelight::Events::EventDispatcher::InvokeListeners<Firelight::Events::PlayerHealthChanged>((void*)health);
}

void PlayerEntity::PlayerHealthUpdated()
{

}
