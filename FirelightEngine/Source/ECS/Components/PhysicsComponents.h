#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../../Maths/Vec3.h"

namespace Firelight::ECS
{
	/// <summary>
	/// Simple physics component used for testing
	/// </summary>
	struct PhysicsComponent : BaseComponent
	{
		Firelight::Maths::Vec3f velocity;

		std::string Output() override
		{
			return "X: " + std::to_string(velocity.x) + ", Y: " + std::to_string(velocity.y);
		}
	};
}
