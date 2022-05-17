#include "PlayerEntity.h"

#include <Source/ECS/Components/AnimationComponent.h>
#include <Source/ECS/Components/RenderingComponents.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include <Source/Graphics/AssetManager.h>
#include <Source/Animation/Animation.h>
#include <Source/ECS/Systems/AnimationSystem.h>

#include "../Components/PlayerComponent.h"
#include "../Components/HealthComponent.h"

PlayerEntity::PlayerEntity(float speed)
{
	GetComponent<Firelight::ECS::IdentificationComponent>()->name = "Player";

	AddComponent<HealthComponent>();

	GetComponent<Firelight::ECS::SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/character_idle.png");
	GetComponent<Firelight::ECS::SpriteComponent>()->pixelsPerUnit = 40;
	GetComponent<Firelight::ECS::SpriteComponent>()->layer = 60;
	AddComponent<Firelight::ECS::AnimationComponent>();

	AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::BoxColliderComponent());
	GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->rect = Firelight::Maths::Rectf(0.0f, -0.5f, 2.0f, 2.2f);
	GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->drawCollider = true;
	GetComponent<Firelight::ECS::StaticComponent>()->isStatic = false;
	
	
	//Firelight::ECS::AnimationSystem::Instance()->Play(this, "PlayerWalkRight");

	AddComponent<PlayerComponent>();
	GetComponent<PlayerComponent>()->speed = speed;
}
