#pragma once
#include <string>
#include <vector>
#include "../ECS/EntityComponentSystem.h"

namespace Firelight::Serialisation
{
	class Serialiser
	{
	public:
		//static void SaveLevelJSON(std::vector<ECS::BaseComponent&> components, std::vector<ECS::EntityID> entities);
		static void SaveSceneJSON();
	private:
		static void WriteToFileJSON(std::string filename, std::string jsonText);

	};
}


