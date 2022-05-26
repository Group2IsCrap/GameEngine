#pragma once

#include "../../Core/AIEntity.h"

class AISnowBearEntity : public AIEntity
{
public:
	AISnowBearEntity();
	AISnowBearEntity(Firelight::ECS::EntityID entityID);
	AISnowBearEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
