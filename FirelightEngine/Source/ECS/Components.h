#pragma once

#include "ECSDefines.h"
#include <string>

namespace Firelight::ECS
{
	/// <summary>
	/// Simple ID component used for testing
	/// </summary>
	struct IdentificationComponent : BaseComponent
	{
		std::string name;
		std::string Output() override
		{
			return name;
		}
	};

	/// <summary>
	/// Simple transform component used for testing
	/// </summary>
	struct TransformComponent : BaseComponent
	{
		int posX;
		int posY;

		std::string Output() override
		{
			return "X: " + std::to_string(posX) + "Y: " + std::to_string(posY);
		}
	};

	/// <summary>
	/// Simple physics component used for testing
	/// </summary>
	struct PhysicsComponent : BaseComponent
	{
		int velX;
		int velY;
		std::string Output() override
		{
			return "X: " + std::to_string(velX) + "Y: " + std::to_string(velY);
		}
	};
}