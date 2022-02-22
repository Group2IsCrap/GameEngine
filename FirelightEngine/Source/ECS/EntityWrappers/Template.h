#pragma once
#include "../EntityComponentSystem.h"

namespace Firelight::ECS
{
	class Template
	{
	public:
		Template();
		~Template();

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
			return EntityComponentSystem::Instance()->GetTemplateComponent<T>(m_templateID, index);
		}

		template<typename T, typename T2>
		T2* GetComponent(int index = 0)
		{
			return EntityComponentSystem::Instance()->GetTemplateComponent<T, T2>(m_templateID, index);
		}

		template<typename T>
		std::vector<T*> GetComponents()
		{
			return EntityComponentSystem::Instance()->GetTemplateComponents<T>(m_templateID);
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
			EntityComponentSystem::Instance()->AddComponentToTemplate<T>(m_templateID, component);
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
			EntityComponentSystem::Instance()->AddComponentToTemplate<T>(m_templateID, ptr);
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
			EntityComponentSystem::Instance()->RemoveComponentFromTemplate<T>(m_templateID, index);
		}

		/// <summary>
		/// Function that returns a bool representing whether a given entity contains a given component type
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns></returns>
		template<typename T>
		bool HasComponent()
		{
			return EntityComponentSystem::Instance()->TemplateHasComponent<T>(m_templateID);
		}

		template<typename T, typename T2>
		bool HasComponent()
		{
			return EntityComponentSystem::Instance()->TemplateHasComponent<T, T2>(m_templateID);
		}

		bool operator==(Template& entity)
		{
			return m_templateID == entity.m_templateID;
		}
		bool operator!=(Template& entity)
		{
			return m_templateID != entity.m_templateID;
		}

		EntityID GetTemplateID();
	private:
		Template(const Template&) = delete;
		Template& operator=(const Template&) = delete;
	protected:
		EntityID m_templateID;
	};
}
