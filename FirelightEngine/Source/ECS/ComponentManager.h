#pragma once

#include "ECSDefines.h"
#include "EntityManager.h"

#include "../Utils/ErrorManager.h"

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
		BaseComponent* GetComponent(ComponentTypeID typeID, EntityID entity, int index = 0)
		{
			return m_componentData[typeID][m_componentMap[typeID][entity][index]];
		}

		/// <summary>
		/// Returns a ptr to a specific component type on the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="typeID"></param>
		/// <param name="entity"></param>
		/// <returns>T*</returns>
		template<typename T>
		T* GetComponent(EntityID entity, int index = 0)
		{
			const char* typeName = typeid(T).name();

			ASSERT_THROW(m_componentTypes.find(typeName) != m_componentTypes.end(), ("Component " + std::string(typeName) + " is not registered"));
			ComponentTypeID typeID = m_componentTypes[typeName];
			if (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end())
			{
				return dynamic_cast<T*>(m_componentData[typeID][m_componentMap[typeID][entity][index]]);
			}
			return nullptr;
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
		void RemoveComponent(EntityID entity, int index = 0)
		{
			const char* typeName = typeid(T).name();

			ASSERT_THROW(m_componentTypes.find(typeName) != m_componentTypes.end(), ("Component " + std::string(typeName) + " is not registered"));
			ComponentTypeID typeID = m_componentTypes[typeName];

			if (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end())
			{
				ASSERT_THROW(index < m_componentMap[typeID][entity].size(), ("Component does not exist at that index"));
				int componentIndex = m_componentMap[typeID][entity][index];
				m_componentData[typeID].erase(m_componentData[typeID].begin() + componentIndex);
				m_componentMap[typeID][entity].erase(m_componentMap[typeID][entity].begin() + index);
				UpdateComponentMap(typeID, index);
				if (m_componentMap[typeID][entity].size() == 0)
				{
					m_componentMap[typeID].erase(entity);
				}
			}
		}

		/// <summary>
		/// Function that returns a bool representing whether a given entity contains a given component type
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="entity"></param>
		/// <returns></returns>
		template<typename T>
		bool HasComponent(EntityID entity)
		{
			const char* typeName = typeid(T).name();

			ASSERT_THROW(m_componentTypes.find(typeName) != m_componentTypes.end(), ("Component " + std::string(typeName) + " is not registered"));
			ComponentTypeID typeID = m_componentTypes[typeName];

			return (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end());
		}

		ComponentManager() = default;
		~ComponentManager() = default;

		std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> GetComponentData();
		const char* GetComponentName(ComponentTypeID typeID);
		void RemoveEntity(EntityID entity);

	private:
		void UpdateComponentMap(ComponentTypeID componentType, int removedIndex);
	private:
		std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> m_componentData;
		std::unordered_map<ComponentTypeID, std::unordered_map<EntityID,std::vector<int>>> m_componentMap;
		std::unordered_map<const char*, ComponentTypeID> m_componentTypes;
		std::unordered_map<ComponentTypeID, const char*> m_componentNames;

		static ComponentTypeID sm_nextComponentID;
	};

}