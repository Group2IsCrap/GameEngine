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

	void SceneManager::LoadScene(int index)
	{
		(void)index;
	}

	void SceneManager::UnloadScene(int index)
	{
		(void)index;
	}
	
	void SceneManager::SetBuildIndex(Scene* scene, int index)
	{
		scene->m_buildIndex = index;
	}

	void SceneManager::Initialize()
	{

	}
}