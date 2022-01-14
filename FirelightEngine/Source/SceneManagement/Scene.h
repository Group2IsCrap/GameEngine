#pragma once

#include <string>
#include <vector>

namespace Firelight::SceneManagement
{
	// TODO : Forward declare entity class


	class Scene
	{
	public:
		std::vector<std::string*> m_entities; // TODO : Change string to Entity Wrapper class

	private:
		template<typename T>
		void OnComponentAdded(std::string, T& component); // TODO : Change string to Entity Wrapper

	public:
		Scene();
		~Scene();

		void CreateEntity(const std::string& name = std::string()); // TODO : Change void to Entity Wrapper class
		void DestroyEntity(std::string entity); // TODO : Change string to Entity Wrapper class

		void Update(double deltaTime);

		void GetMainCamera(); // TODO : Change to Entity Wrapper class

	};
}

