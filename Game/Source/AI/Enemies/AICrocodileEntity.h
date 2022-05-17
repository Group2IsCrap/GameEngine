#pragma once

#include "../../Core/AIEntity.h"

class AICrocodileEntity : public AIEntity
{
public:
	AICrocodileEntity();
	AICrocodileEntity(Firelight::ECS::EntityID entityID);
	AICrocodileEntity(bool isTemplate, Firelight::ECS::EntityID entityID);
};
