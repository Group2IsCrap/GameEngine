#pragma once

#include "../ECS/Entity.h"

#include <vector>

namespace Firelight::SceneManagement
{
	class Scene
	{
	public:
		std::vector<Firelight::ECS::Entity*> m_entities;
		int m_buildIndex;

	private:
		const char* m_sceneName;

	public:
		Scene(const char* sceneName);
		~Scene();

		/// <summary>
		/// Returns the name of the scene
		/// </summary>
		const char* GetName() { return m_sceneName; }

		/// <summary>
		/// Returns the build index of the scene
		/// </summary>
		int GetBuildIndex() { return m_buildIndex; }

		/// <summary>
		/// Is responsible for creating a new entity and send it off to the global system using events.
		/// This is the access point for any entity that has to exist in the current context
		/// </summary>
		/// <returns>The created entity</returns>
		Firelight::ECS::Entity* CreateEntity();

		/// <summary>
		/// Destroys the specified entity
		/// </summary>
		/// <param name="entity"></param>
		void DestroyEntity(Firelight::ECS::Entity* entity);

		/// <summary>
		/// Passes down the update loop to all entities in the current context
		/// </summary>
		/// <param name="deltaTime"></param>
		void Update(double deltaTime);

		/// <summary>
		/// Returns the main camera in the scene
		/// </summary>
		void GetMainCamera(); // TODO : Change to Entity Wrapper class

	};
}

