#pragma once
#include "../EntityComponentSystem.h"
#include "../Components/AudioComponents.h"

namespace Firelight::ECS
{
	class Entity
	{
	public:
		Entity();
		Entity(EntityID entityID);
		Entity(bool isTemplate, EntityID templateID);
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
		T* AddComponent(T* component)
		{
			EntityComponentSystem::Instance()->AddComponent<T>(m_entityID, component);
			return component;
		}

		/// <summary>
		/// Associates a component to the given entity
		/// </summary>
		/// <typeparam name="T">Component Type</typeparam>
		/// <param name="entity"></param>
		/// <param name="component"></param>
		template<typename T>
		T* AddComponent()
		{
			T* ptr = new T();
			EntityComponentSystem::Instance()->AddComponent<T>(m_entityID, ptr);
			return ptr;
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
		void PlayAudioClip();
		void StopAudio();
	private:
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;
	protected:
		void PlaySound_Internal(AudioComponent* audioComponent);
		EntityID m_entityID;
	};
}
