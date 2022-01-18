#include "Scene.h"

namespace Firelight::SceneManagement
{
	Scene::Scene(const char* sceneName) : m_buildIndex(-1)
	{
		m_sceneName = sceneName;
	}

	Scene::~Scene()
	{
		for (auto entity : m_entities)
		{
			entity = nullptr;
			delete entity;
		}

		m_entities.clear();
	}


	Firelight::ECS::Entity* Scene::CreateEntity()
	{
		return new Firelight::ECS::Entity();
	}

	void Scene::DestroyEntity(Firelight::ECS::Entity* entity)
	{

	}

	void Scene::Update(double deltaTime)
	{
		UNREFERENCED_PARAMETER(deltaTime);
	}

	void Scene::GetMainCamera()
	{

	}
}
