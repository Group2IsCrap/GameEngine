#pragma once

#include <Source/ECS/Systems/System.h>
#include <Source/ECS/EntityWrappers/SpriteEntityTemplate.h>
#include "../Player/PlayerEntity.h"

class EntitySpawnerSystem : public Firelight::ECS::System
{
public:
	EntitySpawnerSystem();
	virtual void Update(const Firelight::Utils::Time& time, const bool& isPaused);

private:
	void SetupEntityTemplate();

	void SpawnEnemy();
	void SpawnResource();
	void SetPlayer(void* data);

private:
	Firelight::ECS::SpriteEntityTemplate* enemyTemplate;

	PlayerEntity* m_playerEntity;
};