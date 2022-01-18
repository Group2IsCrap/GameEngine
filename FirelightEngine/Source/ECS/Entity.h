#pragma once
#include "EntityComponentSystem.h"

namespace Firelight::ECS
{
	class Entity
	{
	public:
		Entity();
		Entity(EntityID id);
		~Entity();

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
			return EntityComponentSystem::Instance()->GetComponent<T>(entityID, index);
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
			EntityComponentSystem::Instance()->AddComponent<T>(entityID, component);
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
			EntityComponentSystem::Instance()->AddComponent<T>(entityID, new T());
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
			EntityComponentSystem::Instance()->RemoveComponent<T>(entityID, index);
		}

		/// <summary>
		/// Function that returns a bool representing whether a given entity contains a given component type
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		bool HasComponent()
		{
			return EntityComponentSystem::Instance()->HasComponent<T>(entityID);
		}

		bool operator==(Entity& entity)
		{
			return entityID == entity.entityID;
		}
		bool operator!=(Entity& entity)
		{
			return entityID != entity.entityID;
		}

		EntityID GetEntityID();
		Signature GetSignature();
	private:
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
	protected:
		EntityID entityID;
	};
}
