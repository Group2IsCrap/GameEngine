#pragma once

#include "ECSDefines.h"

#include "EntityManager.h"

#include<vector>
#include<assert.h>
#include<unordered_map>
#include<typeinfo>

namespace Engine::ECS
{
	class ComponentManager
	{
	public:
		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			assert(m_componentTypes.find(typeName) == m_componentTypes.end());

			ComponentTypeID componentType = sm_nextComponentID++;
			m_componentTypes.insert({ typeName, componentType });
			m_componentNames.insert({ componentType, typeName });
			m_componentData.insert({ componentType,std::vector<BaseComponent>() });
		}

		template<typename T>
		ComponentTypeID GetComponentType()
		{
			const char* typeName = typeid(T).name();

			assert(m_componentTypes.find(typeName) != m_componentTypes.end());

			// Return this component's type - used for creating signatures
			return m_componentTypes[typeName];
		}

		template<typename T>
		void AddComponent(Entity entity, T component)
		{
			const char* typeName = typeid(T).name();

			assert(m_componentTypes.find(typeName) != m_componentTypes.end());
			ComponentTypeID typeID = m_componentTypes[typeName];
			m_componentData[typeID].push_back(component);
			int componentIndex = m_componentData.size() - 1;
			if(!m_componentMap[typeID].contains(entity))
			{
				m_componentMap[typeID].insert({ entity,std::vector<int>() });
			}
			m_componentMap[typeID][entity].push_back(componentIndex);
		}

		ComponentManager() = default;
		~ComponentManager() = default;

		std::unordered_map<ComponentTypeID, std::vector<BaseComponent>> GetComponentData();
		const char* GetComponentName(ComponentTypeID typeID);

	private:
		std::unordered_map<ComponentTypeID, std::vector<BaseComponent>> m_componentData;
		std::unordered_map<ComponentTypeID, std::unordered_map<Entity,std::vector<int>>> m_componentMap;
		std::unordered_map<const char*, ComponentTypeID> m_componentTypes;
		std::unordered_map<ComponentTypeID, const char*> m_componentNames;

		static ComponentTypeID sm_nextComponentID;
	};

}