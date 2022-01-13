#pragma once

#include <string>
#include <vector>

namespace Scenes
{
	// TODO : Forward declare entity class


	class Scene
	{
	public:
		std::vector<std::string*> m_entities; // TODO : Change string to Entity class

	private:
		template<typename T>
		void OnComponentAdded(std::string, T& component); // TODO : Change string to Entity

	public:
		Scene();
		~Scene();

		void CreateEntity(const std::string& name = std::string()); // TODO : Change void to Entity class
		void DestroyEntity(std::string entity); // TODO : Change string to Entity class

		void Update(double deltaTime);

		void GetMainCamera(); // TODO : Change to Entity class

	};
}

