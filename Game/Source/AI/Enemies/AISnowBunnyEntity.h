#pragma once

#include "../../Core/AIEntity.h"

class AISnowBunnyEntity : public AIEntity
{
public:
	AISnowBunnyEntity();
	AISnowBunnyEntity(Firelight::ECS::EntityID entityID);
	AISnowBunnyEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
