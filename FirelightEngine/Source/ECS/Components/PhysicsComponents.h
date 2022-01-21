#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../../Maths/Vec3.h"
#include "../../Maths/Rect.h"

namespace Firelight::ECS
{
	/// <summary>
	/// Simple physics component used for testing
	/// </summary>
	struct RigidBodyComponent : BaseComponent
	{
		Firelight::Maths::Vec3f velocity;

		void Serialise() override
		{
			return;
		}
	};

	struct ColliderComponent : BaseComponent
	{
		bool isEnabled = true;
	};

	struct BoxColliderComponent : ColliderComponent
	{
		Firelight::Maths::Rectf rect;
	};

	struct CircleColliderComponent : ColliderComponent
	{
		float radius;
	};
}
