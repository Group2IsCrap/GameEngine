#pragma once

#include "../../Core/AIEntity.h"

class AIDeerEntity : public AIEntity
{
public:
	AIDeerEntity();
	AIDeerEntity(Firelight::ECS::EntityID entityID);
	AIDeerEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
