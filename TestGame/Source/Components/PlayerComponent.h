#pragma once

#include "Source/ECS/ECSDefines.h"

class PlayerComponent : public Firelight::ECS::BaseComponent
{
public:
	float speed = 6.0f;
};
