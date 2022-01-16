#pragma once

#include <cstdint>
#include <string>

namespace Firelight::ECS
{
	typedef std::uint32_t EntityID;
	typedef std::uint16_t ComponentTypeID;

	/// <summary>
	/// Base struct of all components
	/// </summary>
	struct BaseComponent
	{
		/// <summary>
		/// Outputs the contents of the component for debug purposes
		/// </summary>
		/// <returns></returns>
		virtual std::string Output()
		{
			return "";
		}
	};
}