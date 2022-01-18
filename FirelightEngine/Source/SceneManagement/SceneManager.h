#pragma once

#include "Scene.h"

#include <vector>


namespace Firelight::SceneManagement
{

	class SceneManager
	{
	public:

	private:
		
		static SceneManager* sm_instance;

		Scene* m_activeScene;
		std::vector<Scene*> m_scenes;
	public:

		static SceneManager* Instance();

		/// <summary>
		/// Unloads the current scene and loads the new one.
		/// </summary>
		/// <param name="index">Scene buildIndex</param>
		void LoadScene(int index);

		/// <summary>
		/// Deletes memory, possibly where you'd save the game?
		/// </summary>
		void Release();

		/// <summary>
		/// Returns the build index of the scene.
		/// </summary>
		/// <param name="scene"></param>
		/// <returns></returns>
		int GetBuildIndex(Scene* scene);

	private:

		SceneManager();
		~SceneManager() = default;

		/// <summary>
		/// Unloads the scene if it is active
		/// </summary>
		/// <param name="index">Scene buildIndex</param>
		void UnloadScene(int index);

		/// <summary>
		/// Sets the build index of a scene
		/// </summary>
		/// <param name="scene"></param>
		void SetBuildIndex(Scene* scene, int index);

		/// <summary>
		/// Adds all scenes from files, stores them, then loads the scene of index 0
		/// </summary>
		void Initialize();
	};
}