#include "ComponentManager.h"

namespace Firelight::ECS
{
	ComponentTypeID ComponentManager::sm_nextComponentID = 0;

	std::unordered_map<ComponentTypeID, std::vector<BaseComponent>> ComponentManager::GetComponentData()
	{
		return m_componentData;
	}
	const char* ComponentManager::GetComponentName(ComponentTypeID typeID)
	{
		return m_componentNames[typeID];
	}
}