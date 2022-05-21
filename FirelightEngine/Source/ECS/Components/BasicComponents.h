#pragma once

#include <string>

#include "../ECSDefines.h"
#include "../../Maths/Vec3.h"
#include "../../Serialisation/Serialiser.h"

using namespace Firelight::Serialisation;

namespace Firelight::ECS
{
	/// <summary>
	/// Simple ID component used for testing
	/// </summary>
	
	DEFINE_COMPONENT(IdentificationComponent, BaseComponent)
		std::string name;

		void Serialise() override
		{
			Serialiser::Serialise("Name", name.c_str());
		}

		IdentificationComponent* Clone() override
		{
			IdentificationComponent* clone = new IdentificationComponent();
			clone->name = name;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Name", name);
		}
	};

	/// <summary>
	/// Stores whether an entity is considered static or not
	/// </summary>
	struct StaticComponent : BaseComponent
	{
		bool isStatic = false;

		void Serialise() override
		{
			Serialiser::Serialise("IsStatic", isStatic);
		}

		StaticComponent* Clone() override
		{
			StaticComponent* clone = new StaticComponent();
			clone->isStatic = isStatic;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("IsStatic", isStatic ? "true" : "false");
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

		void RenderDebugUI() override
		{
			ImGuiVariable("Layer", layer);
		}

	};

	/// <summary>
	/// Simple transform component used for testing
	/// </summary>
	struct TransformComponent : BaseComponent
	{
		Firelight::Maths::Vec3f position;
		Firelight::Maths::Vec3f scale;
		float                   rotation = 0.0f;

		void Serialise() override
		{
			Serialiser::Serialise("Rotation", rotation);
			Serialiser::Serialise("Position", position);
			Serialiser::Serialise("Scale", scale);
		}

		TransformComponent* Clone() override
		{
			TransformComponent* clone = new TransformComponent();
			clone->position = position;
			clone->scale = scale;
			clone->rotation = rotation;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("Position", position);
			ImGuiVariable("Rotation", rotation);
			ImGuiVariable("Scale", scale);
		}
	};

}
