#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../../Maths/Vec3.h"
#include "../../Maths/Rect.h"

#include "../../Serialisation/Serialiser.h"

using namespace Firelight::Serialisation;

namespace Firelight::ECS
{
	/// <summary>
	/// Simple physics component used for testing
	/// </summary>
	struct RigidBodyComponent : BaseComponent
	{
		Firelight::Maths::Vec3f velocity;
		int layer;

		void Serialise() override
		{
			Serialiser::Serialise("Velocity", velocity);
			Serialiser::Serialise("Layer", layer);
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
		bool drawCollider = false;
		bool isTrigger = false;

		void Serialise() override
		{
			Serialiser::Serialise("IsEnabled", isEnabled);
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

		void Serialise() override
		{
			Serialiser::Serialise("Rect", rect);
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

		void Serialise() override
		{
			Serialiser::Serialise("Radius", radius);
		}

		CircleColliderComponent* Clone() override
		{
			CircleColliderComponent* clone = new CircleColliderComponent();
			clone->radius = radius;

			return clone;
		}
	};
}
