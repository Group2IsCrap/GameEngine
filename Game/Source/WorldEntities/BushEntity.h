#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Events/Event.h>

#include "../Core/ResourceEntity.h"

class BushEntity : public ResourceEntity
{
public:
	BushEntity();
	BushEntity(Firelight::ECS::EntityID entityID);
	BushEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
