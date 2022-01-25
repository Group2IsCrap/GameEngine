#pragma once
#include <string>
#include <vector>
#include "../ECS/EntityComponentSystem.h"
#include "rapidjson/writer.h"

namespace Firelight::Serialisation
{
	class Serialiser
	{
		void WriteToFileJSON(std::string filename, std::string jsonText);

	public:
		void SaveLevelJSON(std::vector<ECS::BaseComponent&> components, std::vector<ECS::EntityID> entities);
	};
}


