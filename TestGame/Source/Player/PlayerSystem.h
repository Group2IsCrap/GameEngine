#pragma once

#include <Source/ECS/Systems/System.h>

class PlayerSystem : public Firelight::ECS::System
{
public:
	PlayerSystem();
	void Update(const Firelight::Utils::Time& time) override;
};