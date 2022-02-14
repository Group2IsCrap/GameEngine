#pragma once

#include "ECSDefines.h"
#include "EntityManager.h"
#include "../Events/EventDispatcher.h"
#include "ECSEvents.h"

#include "../Utils/ErrorManager.h"
#include "rapidjson/prettywriter.h"

#include<vector>
#include<unordered_map>
#include<typeinfo>

namespace Firelight::ECS
{
	class ComponentManager
	{
	public:
		/// <summary>
		/// Returns the component type id of the given typename
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns>ComponentTypeID</returns>
		template<typename T>
		static ComponentTypeID GetComponentType()
		{
			RegisterComponent<T>();

			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			ASSERT_THROW(sm_componentHashTypes.find(typeName) != sm_componentHashTypes.end(), ("Component " + std::string(typeid(T).name()) + " is not registered"));

			// Return this component's type - used for creating signatures
			return sm_componentHashTypes[typeName];
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
			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			if (sm_componentHashTypes.find(typeName) == sm_componentHashTypes.end())
			{
				return nullptr;
			}

			ComponentTypeID typeID = sm_componentHashTypes[typeName];
			if (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end())
			{
				return dynamic_cast<T*>(m_componentData[typeID][m_componentMap[typeID][entity][index]]);
			}
			return nullptr;
		}

		template<typename T, typename T2>
		T2* GetComponent(EntityID entity, int index = 0)
		{
			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			if (sm_componentHashTypes.find(typeName) == sm_componentHashTypes.end())
			{
				return nullptr;
			}

			ComponentTypeID typeID = sm_componentHashTypes[typeName];
			if (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end())
			{
				return dynamic_cast<T2*>(m_componentData[typeID][m_componentMap[typeID][entity][index]]);
			}
			return nullptr;
		}


		/// <summary>
		/// Returns full vector of components of a given type for a given entity
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="entity"></param>
		/// <returns></returns>
		template<typename T>
		std::vector<T*> GetComponents(EntityID entity)
		{
			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			if (sm_componentHashTypes.find(typeName) == sm_componentHashTypes.end())
			{
				return std::vector<T*>();
			}

			ComponentTypeID typeID = sm_componentHashTypes[typeName];
			if (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end())
			{
				std::vector<T*> components = std::vector<T*>();
				for (int i = 0; i < m_componentMap[typeID][entity].size(); ++i)
				{
					components.push_back(dynamic_cast<T*>(m_componentData[typeID][m_componentMap[typeID][entity][i]]));
				}

				return components;
			}
			return std::vector<T*>();
		}

		/// <summary>
		/// Returns all components of a given type
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		std::vector<T*> GetAllComponents()
		{
			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			if (sm_componentHashTypes.find(typeName) == sm_componentHashTypes.end())
			{
				return std::vector<T*>();
			}

			ComponentTypeID typeID = sm_componentHashTypes[typeName];
			std::vector<T*> components = std::vector<T*>();
			for (int i = 0; i < m_componentData[typeID].size(); ++i)
			{
				components.push_back(dynamic_cast<T*>(m_componentData[typeID][i]));
			}
			return components;
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
		/// Associates a component to the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="entity"></param>
		/// <param name="component"></param>
		template<typename T>
		void AddComponent(EntityID entity, T* component)
		{
			RegisterComponent<T>();

			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));
			ASSERT_THROW(sm_componentHashTypes.find(typeName) != sm_componentHashTypes.end(), ("Component " + std::string(typeid(T).name()) + " is not registered"));
			ComponentTypeID typeID = sm_componentHashTypes[typeName];

			if (m_componentData.find(typeID) == m_componentData.end())
			{
				m_componentData.insert({ typeID,std::vector<BaseComponent*>() });
			}

			m_componentData[typeID].push_back(component);
			int componentIndex = (int)m_componentData[typeID].size() - 1;
			if(!m_componentMap[typeID].contains(entity))
			{
				m_componentMap[typeID].insert({ entity,std::vector<int>() });
			}
			m_componentMap[typeID][entity].push_back(componentIndex);
		}
		
		void AddComponent(EntityID entity, ComponentTypeID typeID,  BaseComponent* component)
		{
			if (m_componentData.find(typeID) == m_componentData.end())
			{
				m_componentData.insert({ typeID,std::vector<BaseComponent*>() });
			}

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
			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			if (sm_componentHashTypes.find(typeName) == sm_componentHashTypes.end())
			{
				return;
			}

			ComponentTypeID typeID = sm_componentHashTypes[typeName];

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
			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			if (sm_componentHashTypes.find(typeName) == sm_componentHashTypes.end())
			{
				return false;
			}

			ComponentTypeID typeID = sm_componentHashTypes[typeName];

			return (m_componentMap[typeID].find(entity) != m_componentMap[typeID].end());
		}

		template<typename T, typename T2>
		bool HasComponent(EntityID entity)
		{
			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			if (sm_componentHashTypes.find(typeName) == sm_componentHashTypes.end())
			{
				return false;
			}

			ComponentTypeID typeID = sm_componentHashTypes[typeName];
			auto it = m_componentMap[typeID].find(entity);
			if (it == m_componentMap[typeID].end())
			{
				return false;
			}

			for (int componentIndex : m_componentMap[typeID][entity])
			{
				T2* castComponent = dynamic_cast<T2*>(m_componentData[typeID][componentIndex]);
				if (castComponent != nullptr)
				{
					return true;
				}
			}
			return false;
		}

		ComponentManager() = default;
		~ComponentManager() = default;

		bool HasComponent(ComponentTypeID typeID, EntityID entity);
		int GetRegisteredComponentTypeCount();
		std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> GetComponentData();
		std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> GetComponentDataCopy(EntityID id);
		const char* GetComponentName(ComponentTypeID typeID);
		void RemoveEntity(EntityID entity);

		void SerializeAllComponents(rapidjson::PrettyWriter<rapidjson::StringBuffer>& writer);

	private:

		/// <summary>
		/// Registers the given component so that it's existance is guaranteed
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<typename T>
		static void RegisterComponent()
		{
			TypeHash typeName = std::hash<std::string>{}(std::string(typeid(T).name()));

			if (sm_componentHashTypes.find(typeName) != sm_componentHashTypes.end())
			{
				return;
			}
			while (sm_componentTypeHash.find(sm_nextComponentID) != sm_componentTypeHash.end())
			{
				sm_nextComponentID++;
			}
			ComponentTypeID componentType = sm_nextComponentID++;
			sm_componentHashTypes.insert({ typeName, componentType });
			sm_componentHashNames.insert({ typeName, typeid(T).name() });
			sm_componentTypeHash.insert({ componentType, typeName });

			Events::EventDispatcher::InvokeFunctions<Events::ECS::OnComponentRegisteredEvent>();
		}

		void UpdateComponentMap(ComponentTypeID componentType, int removedIndex);

	private:
		std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> m_componentData;
		std::unordered_map<ComponentTypeID, std::unordered_map<EntityID,std::vector<int>>> m_componentMap;
		static std::unordered_map<TypeHash, ComponentTypeID> sm_componentHashTypes;
		static std::unordered_map<ComponentTypeID, TypeHash> sm_componentTypeHash;
		static std::unordered_map<TypeHash, const char*> sm_componentHashNames;

		static ComponentTypeID sm_nextComponentID;
	};


}