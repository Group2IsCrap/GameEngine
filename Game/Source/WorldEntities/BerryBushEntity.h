#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Events/Event.h>

#include "../Core/ResourceEntity.h"

class BerryBushEntity : public ResourceEntity
{
public:
	BerryBushEntity();
	BerryBushEntity(Firelight::ECS::EntityID entityID);
	BerryBushEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
