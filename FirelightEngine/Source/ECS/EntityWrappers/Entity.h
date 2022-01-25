#pragma once
#include "../EntityComponentSystem.h"

namespace Firelight::ECS
{
	class Entity
	{
	public:
		Entity();
		Entity(EntityID id);
		~Entity();

		void Destroy();

		/// <summary>
		/// Returns a ptr to a specific component type on the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="typeID"></param>
		/// <param name="entity"></param>
		/// <returns>T*</returns>
		template<typename T>
		T* GetComponent(int index = 0)
		{
			return EntityComponentSystem::Instance()->GetComponent<T>(m_entityID, index);
		}

		template<typename T, typename T2>
		T2* GetComponent(int index = 0)
		{
			return EntityComponentSystem::Instance()->GetComponent<T,T2>(m_entityID, index);
		}
		
		template<typename T>
		std::vector<T*> GetComponents()
		{
			return EntityComponentSystem::Instance()->GetComponents<T>(m_entityID);
		}

		/// <summary>
		/// Associates a component to the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="entity"></param>
		/// <param name="component"></param>
		template<typename T>
		void AddComponent(T* component)
		{
			EntityComponentSystem::Instance()->AddComponent<T>(m_entityID, component);
		}

		/// <summary>
		/// Associates a component to the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="entity"></param>
		/// <param name="component"></param>
		template<typename T>
		void AddComponent()
		{
			EntityComponentSystem::Instance()->AddComponent<T>(m_entityID, new T());
		}

		/// <summary>
		/// Disassociates a component of type T with the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="entity"></param>
		/// <param name="component"></param>
		template<typename T>
		void RemoveComponent(int index = 0)
		{
			EntityComponentSystem::Instance()->RemoveComponent<T>(m_entityID, index);
		}

		/// <summary>
		/// Function that returns a bool representing whether a given entity contains a given component type
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		bool HasComponent()
		{
			return EntityComponentSystem::Instance()->HasComponent<T>(m_entityID);
		}

		template<typename T, typename T2>
		bool HasComponent()
		{
			return EntityComponentSystem::Instance()->HasComponent<T, T2>(m_entityID);
		}

		bool operator==(Entity& entity)
		{
			return m_entityID == entity.m_entityID;
		}
		bool operator!=(Entity& entity)
		{
			return m_entityID != entity.m_entityID;
		}

		EntityID GetEntityID();
		Signature GetSignature();
	private:
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
	protected:
		EntityID m_entityID;
	};
}
