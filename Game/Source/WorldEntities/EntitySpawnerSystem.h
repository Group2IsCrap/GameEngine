#pragma once

#include <Source/ECS/Systems/System.h>
#include <Source/ECS/EntityWrappers/SpriteEntityTemplate.h>

class EntitySpawnerSystem : public Firelight::ECS::System
{
public:
	EntitySpawnerSystem();
	virtual void Update(const Firelight::Utils::Time& time, const bool& isPaused);

private:
	void SetupEntityTemplate();

private:
	Firelight::ECS::SpriteEntityTemplate* enemyTemplate;
};