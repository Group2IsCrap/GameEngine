#pragma once
#include "../ECS/ECSDefines.h"
#include <string>



// Simple physics component used for testing
struct PhysicsComponent : Firelight::ECS::BaseComponent
{
	int posX, posY, Height, Width, Radius;
	bool hasBoxCollision, HasCircleCollision

		std::string Output() override
	{
		return "X: " + std::to_string(posX) + "Y: " + std::to_string(posY);
	}
};
