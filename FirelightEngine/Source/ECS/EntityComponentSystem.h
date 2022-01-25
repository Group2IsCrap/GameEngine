#pragma once

#include "EntityManager.h"
#include "ComponentManager.h"

#include <memory>

namespace Firelight::ECS
{
	class EntityComponentSystem
	{
	public:
		/// <summary>
		/// Registers the given component so that it's existance is guaranteed
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<typename T>
		void RegisterComponent()
		{
			m_componentManager->RegisterComponent<T>();
		}

		/// <summary>
		/// Returns the component type id of the given typename
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns>ComponentTypeID</returns>
		template<typename T>
		ComponentTypeID GetComponentType()
		{
			return m_componentManager->GetComponentType<T>();
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
			return m_componentManager->GetComponent<T>(entity, index);
		}

		template<typename T, typename T2>
		T2* GetComponent(EntityID entity, int index = 0)
		{
			return m_componentManager->GetComponent<T,T2>(entity, index);
		}

		/// <summary>
		/// Returns all components of a given type for a given entity
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="entity"></param>
		/// <returns></returns>
		template<typename T>
		std::vector<T*> GetComponents(EntityID entity)
		{
			return m_componentManager->GetComponents<T>(entity);
		}

		/// <summary>
		/// Get all components of a given type
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		std::vector<T*> GetAllComponents()
		{
			return m_componentManager->GetAllComponents<T>();
		}

		/// <summary>
		/// Returns a list of all components.
		/// </summary>
		/// <returns>std::vector<BaseComponent*></returns>
		std::vector<BaseComponent*> GetAllComponents()
		{
			std::vector<BaseComponent*> values;
			std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>> componentData = m_componentManager->GetComponentData();
			for (std::unordered_map<ComponentTypeID, std::vector<BaseComponent*>>::iterator it = componentData.begin(); it != componentData.end(); ++it)
			{
				for (auto c : it->second)
				{
					values.push_back(c);
				}
			}

			return values;
		}

		/// <summary>
		/// Returns a ptr to a BaseComponent on the given entity
		/// </summary>
		/// <param name="typeID"></param>
		/// <param name="entity"></param>
		/// <returns>BaseComponent*</returns>
		BaseComponent* GetComponent(ComponentTypeID typeID, EntityID entity, int index = 0)
		{
			return m_componentManager->GetComponent(typeID, entity, index);
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
			m_componentManager->AddComponent<T>(entity, component);
			m_entityManager->UpdateEntitySignature(entity, m_componentManager->GetComponentType<T>(), true);
			Events::EventDispatcher::InvokeFunctions<Events::ECS::OnComponentAddedEvent>();
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
			m_componentManager->RemoveComponent<T>(entity, index);
			if (!HasComponent<T>(entity))
			{
				m_entityManager->UpdateEntitySignature(entity, m_componentManager->GetComponentType<T>(), false);
				Events::EventDispatcher::InvokeFunctions<Events::ECS::OnComponentRemovedEvent>();
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
			return m_componentManager->HasComponent<T>(entity);
		}

		template<typename T, typename T2>
		bool HasComponent(EntityID entity)
		{
			return m_componentManager->HasComponent<T, T2>(entity);
		}

		/// <summary>
		/// Removes an entity
		/// </summary>
		/// <param name="entity"></param>
		void RemoveEntity(EntityID entity)
		{
			m_entityManager->RemoveEntity(entity);

			m_componentManager->RemoveEntity(entity);
		}

		EntityID CreateEntity();
		EntityID CreateEntity(EntityID id);
		std::vector<EntityID> GetEntities();
		Signature GetSignature(EntityID entityID);

		int GetNumberOfComponents();
		void UpdateAllEntitySignatures();

		static EntityComponentSystem* Instance();
	private:
		EntityComponentSystem();
		~EntityComponentSystem() = default;
	private:
		static EntityComponentSystem* sm_instance;

		std::unique_ptr<EntityManager> m_entityManager;
		std::unique_ptr<ComponentManager> m_componentManager;
	};
}