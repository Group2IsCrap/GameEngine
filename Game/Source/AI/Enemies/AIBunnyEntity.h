#pragma once

#include "../../Core/AIEntity.h"

class AIBunnyEntity : public AIEntity
{
public:
	AIBunnyEntity();
	AIBunnyEntity(Firelight::ECS::EntityID entityID);
	AIBunnyEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
