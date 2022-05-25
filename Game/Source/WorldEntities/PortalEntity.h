#pragma once
#include <Source/ECS/ECSDefines.h>
#include <Source/ECS/EntityWrappers/Entity.h>
#include <Source/ECS/Components/AudioComponents.h>
#include <Source/Engine.h>
#include <Source/ECS/EntityWrappers/SpriteEntity.h>
#include <Source/ECS/Components/AnimationComponent.h>
#include <Source/ECS/Systems/AnimationSystem.h>
#include <Source/Graphics/AssetManager.h>
#include <Source/Maths/Vec3.h>
#include "../Events/GameEvents.h"
#include "../Core/Layers.h"

using namespace Firelight::ECS;

class PortalEntity : public Firelight::ECS::Entity
{
public:
	PortalEntity();
	PortalEntity(std::string name);
	PortalEntity(Firelight::ECS::EntityID entityID);
	PortalEntity(bool isTemplate, Firelight::ECS::EntityID entityID);

	SpriteComponent* sprite;
	TransformComponent* transformComponent;
	AnimationComponent* animationComponent;

	void Play();

	AudioComponent* audioComponent;

	size_t m_playEventIndex;

};