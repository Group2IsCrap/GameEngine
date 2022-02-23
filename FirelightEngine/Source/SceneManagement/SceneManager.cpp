#include "SceneManager.h"

namespace Firelight::SceneManagement
{
	SceneManager* SceneManager::sm_instance = nullptr;

	SceneManager* SceneManager::Instance()
	{
		if (sm_instance == nullptr)
		{
			sm_instance = new SceneManager();
		}
		return sm_instance;
	}

	int SceneManager::GetBuildIndex(Scene* scene)
	{
		return scene->GetBuildIndex();
	}

	SceneManager::SceneManager()
	{
		Initialize();
	}

	void SceneManager::Release()
	{
		for (auto scene : m_scenes)
		{
			delete scene;
			scene = nullptr;
		}

		m_scenes.clear();

		delete sm_instance;
		sm_instance = nullptr;
	}

	//First unloads active scene & then loads scene with entities coresponding with index
	void SceneManager::LoadScene(int index)
	{
		UNREFERENCED_PARAMETER(index);
		//Max S edit. First unloads active scene before loading new scene
		UnloadScene(m_activeScene);


	}

	//Max S edit. Changed so a scene pointer is passed in as opposed to index. Allows for active scene to be unloaded directly rather than searching for index beforehand.
	void SceneManager::UnloadScene(Scene* scene)
	{
		UNREFERENCED_PARAMETER(index);
	}
	
	void SceneManager::SetBuildIndex(Scene* scene, int index)
	{
		scene->m_buildIndex = index;
	}

	void SceneManager::Initialize()
	{
		LoadScene(0); //Max S edit. Load scene with index 0. Main Menu probably?
	}
}