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

	Firelight::Animation::Animation idle = Firelight::Animation::Animation(
		"PlayerIdle",
		Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/character_idle.png"),
		96,
		128,
		1,
		100.0f);

	Firelight::Animation::Animation walkRight = Firelight::Animation::Animation(
		"PlayerWalkRight",
		Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/character_walk.png"),
		96,
		128,
		8,
		100.0f);

	Firelight::Animation::Animation walkLeft = Firelight::Animation::Animation(
		"PlayerWalkLeft",
		Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/character_walk_left.png"),
		96,
		128,
		8,
		100.0f);

	GetComponent<Firelight::ECS::SpriteComponent>()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/character_idle.png");
	GetComponent<Firelight::ECS::SpriteComponent>()->pixelsPerUnit = 40;
	GetComponent<Firelight::ECS::SpriteComponent>()->layer = 60;
	AddComponent<Firelight::ECS::AnimationComponent>();
	GetComponent<Firelight::ECS::AnimationComponent>()->animations.insert(std::pair<std::string, Firelight::Animation::Animation>(idle.m_animationName, idle));
	GetComponent<Firelight::ECS::AnimationComponent>()->animations.insert(std::pair<std::string, Firelight::Animation::Animation>(walkRight.m_animationName, walkRight));
	GetComponent<Firelight::ECS::AnimationComponent>()->animations.insert(std::pair<std::string, Firelight::Animation::Animation>(walkLeft.m_animationName, walkLeft));

	AddComponent<Firelight::ECS::ColliderComponent>(new Firelight::ECS::BoxColliderComponent());
	GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->rect = Firelight::Maths::Rectf(0.0f, -0.5f, 2.0f, 2.2f);
	GetComponent<Firelight::ECS::ColliderComponent, Firelight::ECS::BoxColliderComponent>()->drawCollider = true;
	GetComponent<Firelight::ECS::StaticComponent>()->isStatic = false;
	
	
	//Firelight::ECS::AnimationSystem::Instance()->Play(this, "PlayerWalkRight");

	AddComponent<PlayerComponent>();
	GetComponent<PlayerComponent>()->speed = speed;
}
