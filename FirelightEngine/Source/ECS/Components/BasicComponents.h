#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../../Maths/Vec3.h"

namespace Firelight::ECS
{
	/// <summary>
	/// Simple ID component used for testing
	/// </summary>
	struct IdentificationComponent : BaseComponent
	{
		std::string name;

		void Serialise() override
		{
			return;
		}
	};

	/// <summary>
	/// Stores whether an entity is considered static or not
	/// </summary>
	struct StaticComponent : BaseComponent
	{
		bool isStatic;

		void Serialise() override
		{
			return;
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

		void Serialise() override
		{
			return;
		}
	};
}
