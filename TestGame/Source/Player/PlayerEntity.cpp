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

	Firelight::Animation::Animation animation = Firelight::Animation::Animation(
		"Shrek",
		Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/shrekWalk.png"),
		210,
		259,
		6,
		100.0f);

	GetComponent<Firelight::ECS::SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/shrekWalk.png");
	GetComponent<Firelight::ECS::SpriteComponent>()->pixelsPerUnit = 200;
	GetComponent<Firelight::ECS::SpriteComponent>()->layer = 64;
	AddComponent<Firelight::ECS::AnimationComponent>();
	GetComponent<Firelight::ECS::AnimationComponent>()->animations.insert(std::pair<std::string, Firelight::Animation::Animation>(animation.m_animationName, animation));

	AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::BoxColliderComponent());
	GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->rect = Firelight::Maths::Rectf(0.0f, 0.0f, 1.0f, 1.0f);
	Firelight::ECS::AnimationSystem::Instance()->Play(this, "Shrek");

	AddComponent<PlayerComponent>();
	GetComponent<PlayerComponent>()->speed = speed;
}
