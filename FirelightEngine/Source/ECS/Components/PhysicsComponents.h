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

		RigidBodyComponent* Clone() override
		{
			RigidBodyComponent* clone = new RigidBodyComponent();
			clone->velocity = velocity;
			clone->layer = layer;

			return clone;
		}
	};

	struct ColliderComponent : BaseComponent
	{
		bool isEnabled = true;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			return;
		}

		ColliderComponent* Clone() override
		{
			ColliderComponent* clone = new ColliderComponent();
			clone->isEnabled = isEnabled;

			return clone;
		}
	};

	struct BoxColliderComponent : ColliderComponent
	{
		Firelight::Maths::Rectf rect;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			return;
		}

		BoxColliderComponent* Clone() override
		{
			BoxColliderComponent* clone = new BoxColliderComponent();
			clone->rect = rect;

			return clone;
		}
	};

	struct CircleColliderComponent : ColliderComponent
	{
		float radius;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			return;
		}

		CircleColliderComponent* Clone() override
		{
			CircleColliderComponent* clone = new CircleColliderComponent();
			clone->radius = radius;

			return clone;
		}
	};
}
