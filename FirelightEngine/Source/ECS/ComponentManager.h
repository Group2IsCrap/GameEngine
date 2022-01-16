#pragma once

#include "ECSDefines.h"

#include "EntityManager.h"

#include<vector>
#include<unordered_map>
#include<typeinfo>

namespace Firelight::ECS
{
	class ComponentManager
	{
	public:
		/// <summary>
		/// Registers the given component so that it's existance is guaranteed
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<typename T>
		void RegisterComponent()
		{
			const char* typeName = typeid(T).name();

			ASSERT_THROW(m_componentTypes.find(typeName) == m_componentTypes.end(), ("Component " + std::string(typeName) + " is already registered"));

			ComponentTypeID componentType = sm_nextComponentID++;
			m_componentTypes.insert({ typeName, componentType });
			m_componentNames.insert({ componentType, typeName });
			m_componentData.insert({ componentType,std::vector<BaseComponent*>() });
		}

		/// <summary>
		/// Returns the component type id of the given typename
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns>ComponentTypeID</returns>
		template<typename T>
		ComponentTypeID GetComponentType()
		{
			const char* typeName = typeid(T).name();

			ASSERT_THROW(m_componentTypes.find(typeName) != m_componentTypes.end(), ("Component " + std::string(typeName) + " is not registered"));

			// Return this component's type - used for creating signatures
			return m_componentTypes[typeName];
		}

		/// <summary>
		/// Returns a ptr to a BaseComponent on the given entity
		/// </summary>
		/// <param name="typeID"></param>
		/// <param name="entity"></param>
		/// <returns>BaseComponent*</returns>
		BaseComponent* GetComponent(ComponentTypeID typeID, EntityID entity)
		{
			return m_componentData[typeID][entity];
		}

		/// <summary>
		/// Returns a ptr to a specific component type on the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="typeID"></param>
		/// <param name="entity"></param>
		/// <returns>T*</returns>
		template<typename T>
		T* GetComponent(EntityID entity)
		{
			const char* typeName = typeid(T).name();

			ASSERT_THROW(m_componentTypes.find(typeName) != m_componentTypes.end(), ("Component " + std::string(typeName) + " is not registered"));
			ComponentTypeID typeID = m_componentTypes[typeName];
			return dynamic_cast<T*>(m_componentData[typeID][entity]);
		}

		/// <summary>
		/// Associates a component to the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="entity"></param>
		/// <param name="component"></param>
		template<typename T>
		void AddComponent(EntityID entity, T* component)
		{
			const char* typeName = typeid(T).name();

			ASSERT_THROW(m_componentTypes.find(typeName) != m_componentTypes.end(), ("Component " + std::string(typeName) + " is not registered"));
			ComponentTypeID typeID = m_componentTypes[typeName];
			m_componentData[typeID].push_back(component);
			int componentIndex = (int)m_componentData[typeID].size() - 1;
			if(!m_componentMap[typeID].contains(entity))
			{
				m_componentMap[typeID].insert({ entity,std::vector<int>() });
			}
			m_componentMap[typeID][entity].push_back(componentIndex);
		}

		/// <summary>
		/// Disassociates a component of type T with the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="entity"></param>
		/// <param name="component"></param>
		template<typename T>
		void RemoveComponent(EntityID entity)
		{
			const char* typeName = typeid(T).name();

			ASSERT_THROW(m_componentTypes.find(typeName) != m_componentTypes.end(), ("Component " + std::string(typeName) + " is not registered"));
			ComponentTypeID typeID = m_componentTypes[typeName];

			if (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end())
			{
				int index = m_componentMap[typeID][entity][0];
				m_componentData[typeID].erase(m_componentData[typeID].begin() + index);
				m_componentMap[typeID].erase(entity);
			}
		}

		/// <summary>
		/// Removes all components associated with the given entity
		/// </summary>
		/// <param name="entity"></param>
		void RemoveEntity(EntityID entity)
		{
			for (auto& componentType : m_componentData)
			{
				if (m_componentMap[componentType.first].find(entity) != m_componentMap[componentType.first].end())
				{
					std::vector<int> componentIndexesToRemove = m_componentMap[componentType.first][entity];
					for (auto& index : componentIndexesToRemove)
					{
						delete m_componentData[componentType.first][index];
						m_componentData[componentType.first].erase(m_componentData[componentType.first].begin() + index);
					}
					m_componentMap[componentType.first].erase(entity);
				}
			}
		}

		ComponentManager() = default;
		~ComponentManager() = default;

		std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> GetComponentData();
		const char* GetComponentName(ComponentTypeID typeID);

	private:
		std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> m_componentData;
		std::unordered_map<ComponentTypeID, std::unordered_map<EntityID,std::vector<int>>> m_componentMap;
		std::unordered_map<const char*, ComponentTypeID> m_componentTypes;
		std::unordered_map<ComponentTypeID, const char*> m_componentNames;

		static ComponentTypeID sm_nextComponentID;
	};

}