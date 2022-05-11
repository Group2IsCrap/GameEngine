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
	GetSpriteComponent()->layer = 60;

	Firelight::ECS::CircleColliderComponent* collider = dynamic_cast<Firelight::ECS::CircleColliderComponent*>(AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::CircleColliderComponent()));
	collider->drawCollider = true;
	collider->radius = 0.75f;
	collider->offset = Firelight::Maths::Vec2f(0.0f,0.0f);
	Firelight::ECS::BoxColliderComponent* boxCollider = dynamic_cast<Firelight::ECS::BoxColliderComponent*>(AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::BoxColliderComponent()));
	boxCollider->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 1.0f, 2.0f);
	boxCollider->drawCollider = true;
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
