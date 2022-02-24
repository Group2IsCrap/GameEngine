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
	/// Simple layer component
	/// </summary>
	struct LayerComponent : BaseComponent
	{
		// Layer 0 = Default
		int layer = 0;

		void Serialise() override
		{
			Serialiser::Serialise("Layer", layer);
		}

		LayerComponent* Clone() override
		{
			LayerComponent* clone = new LayerComponent();
			clone->layer = layer;

			return clone;
		}
	};

	/// <summary>
	/// Simple transform component used for testing
	/// </summary>
	struct TransformComponent : BaseComponent
	{
		Firelight::Maths::Vec3f position;
		Firelight::Maths::Vec3f scale;
		float                   rotation;

		void Serialise() override
		{
			return;
		}
	};

}
