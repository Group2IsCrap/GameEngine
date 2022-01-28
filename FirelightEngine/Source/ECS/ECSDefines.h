#pragma once

#include <cstdint>
#include <string>

#include "rapidjson/prettywriter.h"

namespace Firelight::ECS
{
	typedef std::uint32_t EntityID;
	typedef std::uint16_t ComponentTypeID;
	typedef size_t TypeHash;

	/// <summary>
	/// Base struct of all components
	/// </summary>
	struct BaseComponent
	{
		/// <summary>
		/// Serialises the component. DO NOT NEED TO PUT START OBJECT
		/// </summary>
		/// <returns></returns>
		virtual void Serialise(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer)
		{
			return;
		}
	};
}
