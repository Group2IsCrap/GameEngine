#pragma once
#include <Source/ECS/Systems/System.h>
#include <Source/ECS/EntityWrappers/GameEntity.h>

#include "../CoreComponents/AIComponent.h"

using namespace Firelight::ECS;

class AISystem : public Firelight::ECS::System
{
public:
	AISystem();
	~AISystem();

	void Update(const Firelight::Utils::Time& time, const bool& isPaused) override;
};

