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
		T* GetComponent(EntityID entity)
		{
			return m_componentManager->GetComponent<T>(entity);
		}

		/// <summary>
		/// Returns a ptr to a BaseComponent on the given entity
		/// </summary>
		/// <param name="typeID"></param>
		/// <param name="entity"></param>
		/// <returns>BaseComponent*</returns>
		BaseComponent* GetComponent(ComponentTypeID typeID, EntityID entity)
		{
			return m_componentManager->GetComponent(typeID, entity);
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
			m_componentManager->RemoveComponent<T>(entity);
			m_entityManager->UpdateEntitySignature(entity, m_componentManager->GetComponentType<T>(), false);
		}

		/// <summary>
		/// Creates a new entity
		/// </summary>
		/// <returns></returns>
		EntityID CreateEntity()
		{
			return m_entityManager->CreateEntity();
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

		void DebugEntities();

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