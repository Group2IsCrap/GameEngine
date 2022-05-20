#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/Events/Event.h>

#include "../Core/ResourceEntity.h"


class TreeEntity : public ResourceEntity
{
public:
	TreeEntity();
	TreeEntity(Firelight::ECS::EntityID entityID);
	TreeEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
