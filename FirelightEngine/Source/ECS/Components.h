#pragma once

#include "ECSDefines.h"
#include "../Maths/Vec3.h"
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
		Firelight::Maths::Vec3f position;
		// TODO : Add some form of rotation, unsure what everyone wants.
		Firelight::Maths::Vec3f scale;

		std::string Output() override
		{
			return "X: " + std::to_string(position.x) + ", Y: " + std::to_string(position.y) + ", Z: " + std::to_string(position.z);
		}
	};

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