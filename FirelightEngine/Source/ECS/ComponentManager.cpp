#include "ComponentManager.h"

namespace Firelight::ECS
{
	ComponentTypeID ComponentManager::sm_nextComponentID = 0;

	/// <summary>
	/// Returns the entire map of component data
	/// </summary>
	/// <returns></returns>
	std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> ComponentManager::GetComponentData()
	{
		return m_componentData;
	}

	/// <summary>
	/// Returns the name of the given component ID
	/// </summary>
	/// <param name="typeID"></param>
	/// <returns></returns>
	const char* ComponentManager::GetComponentName(ComponentTypeID typeID)
	{
		return m_componentNames[typeID];
	}
}