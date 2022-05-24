#pragma once

#include "../../Core/AIEntity.h"

class AISlimeEntity : public AIEntity
{
public:
	AISlimeEntity();
	AISlimeEntity(Firelight::ECS::EntityID entityID);
	AISlimeEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};

