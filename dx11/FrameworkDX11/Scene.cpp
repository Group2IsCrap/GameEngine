#include "Scene.h"

namespace Scenes
{
	Scenes::Scene::Scene()
	{

	}

	Scenes::Scene::~Scene()
	{

	}

	void Scene::CreateEntity(const std::string& name)
	{

	}

	void Scene::DestroyEntity(std::string entity)
	{

	}

	void Scene::Update(double deltaTime)
	{

	}

	void Scene::GetMainCamera()
	{

	}

	template<typename T>
	void Scene::OnComponentAdded(std::string, T& component)
	{
		static_assert(false);
	}
}
