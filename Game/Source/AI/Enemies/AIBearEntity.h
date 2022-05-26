#pragma once

#include "../../Core/AIEntity.h"

class AIBearEntity : public AIEntity
{
public:
	AIBearEntity();
	AIBearEntity(Firelight::ECS::EntityID entityID);
	AIBearEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
