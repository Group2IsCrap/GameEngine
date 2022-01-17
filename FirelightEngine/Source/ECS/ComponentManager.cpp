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

	/// <summary>
	/// When a component is removed, the position of other components is shifted. 
	/// This needs to be reflected in the component map, else the map will become invalidated
	/// </summary>
	/// <param name="componentType"></param>
	/// <param name="removedIndex">The index of the removed component</param>
	void ComponentManager::UpdateComponentMap(ComponentTypeID componentType, int removedIndex)
	{
		for (auto& map : m_componentMap[componentType])
		{
			for (auto& index : map.second)
			{
				if (index > removedIndex)
				{
					index--;
				}
			}
		}
	}
}