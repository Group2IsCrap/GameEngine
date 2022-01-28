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

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			writer.Key("Name");
			writer.String(name.c_str());
		}
	};

	/// <summary>
	/// Stores whether an entity is considered static or not
	/// </summary>
	struct StaticComponent : BaseComponent
	{
		bool isStatic;

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			writer.Key("IsStatic");
			writer.Bool(isStatic);
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

		void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer) override
		{
			/// Serialize Position
			/// Serialize Scale
			writer.Key("Rotation");
			writer.Double(rotation);
		}
	};

}
