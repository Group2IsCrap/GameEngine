#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Events/Event.h>

#include "../Core/ResourceEntity.h"

class RockEntity : public ResourceEntity
{
public:
	RockEntity();
	RockEntity(Firelight::ECS::EntityID entityID);
	RockEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
