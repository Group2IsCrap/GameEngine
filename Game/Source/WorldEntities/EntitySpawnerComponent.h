#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Serialisation/Serialiser.h>
#include <Source/ECS/EntityWrappers/Entity.h>

#include <string>

using namespace Firelight::Serialisation;
using namespace Firelight::ECS;

struct EntitySpawnerComponent : Firelight::ECS::BaseComponent
{
public:
	int resourceID = -1;
	std::string enemyName;

	Entity* spawnedEntity;

	double respawnCooldown;
	double respawnTimer;

	EntitySpawnerComponent* Clone() override
	{
		EntitySpawnerComponent* clone = new EntitySpawnerComponent();
		clone->resourceID = resourceID;
		clone->enemyName = enemyName;
		clone->respawnCooldown = respawnCooldown;
		clone->respawnTimer = respawnTimer;

		return clone;
	}

	void RenderDebugUI() override
	{
		ImGuiVariable("ResourceID", resourceID == -1 ? "N/A" : std::to_string(resourceID));
		ImGuiVariable("EnemyName", enemyName == "" ? "N/A" : enemyName);
		ImGuiVariable("Respawn Cooldown", respawnCooldown);
		ImGuiVariable("Respawn Timer", respawnTimer);
	}
};
