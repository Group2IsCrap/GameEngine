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
		Firelight::Maths::Vec3f lastPos;
		Firelight::Maths::Vec3f nextPos;
		bool interpolate = false;
		float interpolationTime = 0.0f;
		float dragCoefficient = 5.0f;
		int layer;

		void Serialise() override
		{
			Serialiser::Serialise("Velocity", velocity);
			Serialiser::Serialise("LastPos", lastPos);
			Serialiser::Serialise("NextPos", nextPos);
			Serialiser::Serialise("Layer", layer);
			Serialiser::Serialise("Interpolate", interpolate);
			Serialiser::Serialise("InterpolationTime", interpolationTime);
		}

		RigidBodyComponent* Clone() override
		{
			RigidBodyComponent* clone = new RigidBodyComponent();
			clone->velocity = velocity;
			clone->lastPos = lastPos;
			clone->interpolationTime = interpolate;
			clone->interpolate = interpolationTime;
			clone->nextPos = nextPos;
			clone->layer = layer;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Velocity", velocity);
			ImGuiVariable("LastPos", lastPos);
			ImGuiVariable("InterpolationTime", interpolationTime);
			ImGuiVariable("Interpolate", interpolate ? "true" : "false");
			ImGuiVariable("NextPos", nextPos);
			ImGuiVariable("Layer", layer);
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
			clone->drawCollider = drawCollider;
			clone->isTrigger = isTrigger;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("IsEnabled", isEnabled ? "true" : "false");
			ImGuiVariable("DrawCollider", drawCollider);
			ImGuiVariable("IsTrigger", isTrigger ? "true" : "false");
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
			clone->isEnabled = isEnabled;
			clone->drawCollider = drawCollider;
			clone->isTrigger = isTrigger;
			clone->rect = rect;

			return clone;
		}

		void RenderDebugUI() override
		{
			ColliderComponent::RenderDebugUI();
			ImGuiVariable("IsEnabled", isEnabled ? "true" : "false");
		}
	};

	struct CircleColliderComponent : ColliderComponent
	{
		float radius;
		Firelight::Maths::Vec2f offset;

		void Serialise() override
		{
			Serialiser::Serialise("Radius", radius);
			Serialiser::Serialise("Offset", offset);
		}

		CircleColliderComponent* Clone() override
		{
			CircleColliderComponent* clone = new CircleColliderComponent();
			clone->isEnabled = isEnabled;
			clone->drawCollider = drawCollider;
			clone->isTrigger = isTrigger;
			clone->radius = radius;
			clone->offset = offset;

			return clone;
		}

		void RenderDebugUI() override
		{
			ColliderComponent::RenderDebugUI();
			ImGuiVariable("Radius", radius);
			ImGuiVariable("Offset", offset);
		}
	};
}
