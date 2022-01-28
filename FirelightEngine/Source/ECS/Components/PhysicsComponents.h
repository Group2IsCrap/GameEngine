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
		int layer;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			///Serialize Velocity
			writer.Key("Layer");
			writer.Int(layer);
		}
	};

	struct ColliderComponent : BaseComponent
	{
		bool isEnabled = true;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			return;
		}
	};

	struct BoxColliderComponent : ColliderComponent
	{
		Firelight::Maths::Rectf rect;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			return;
		}
	};

	struct CircleColliderComponent : ColliderComponent
	{
		float radius;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			return;
		}
	};
}
