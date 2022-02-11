#pragma once

#include "Source/ECS/ECSDefines.h"

class HealthComponent : public Firelight::ECS::BaseComponent
{
public:
	int health = 3;
};