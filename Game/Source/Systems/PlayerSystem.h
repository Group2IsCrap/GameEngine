#pragma once

#include <Source/ECS/Systems/System.h>

#include "../Player/PlayerEntity.h"

class PlayerSystem : public Firelight::ECS::System
{
public:
	PlayerSystem();
	void Update(const Firelight::Utils::Time& time) override;

private:
	PlayerEntity* playerEntity;
};