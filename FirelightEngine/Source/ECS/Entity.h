#pragma once
#include "EntityComponentSystem.h"

namespace Firelight::ECS
{
	class Entity
	{
	public:
		Entity();
		~Entity();

		/// <summary>
		/// Returns a ptr to a specific component type on the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="typeID"></param>
		/// <param name="entity"></param>
		/// <returns>T*</returns>
		template<typename T>
		T* GetComponent()
		{
			return EntityComponentSystem::Instance()->GetComponent<T>(this->entityID);
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
			EntityComponentSystem::Instance()->AddComponent<T>(this->entityID, component);
		}

		/// <summary>
		/// Disassociates a component of type T with the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="entity"></param>
		/// <param name="component"></param>
		template<typename T>
		void RemoveComponent()
		{
			EntityComponentSystem::Instance()->RemoveComponent<T>(this->entityID);
		}

		EntityID GetEntityID();
		Signature GetSignature();

	protected:
		EntityID entityID;
	};
}
