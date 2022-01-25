#include "Serialiser.h"
#include <fstream>

namespace Firelight::Serialisation
{
	// Creates/Overwirtes file
	void Serialiser::WriteToFileJSON(std::string filename, std::string jsonText)
	{
		std::ofstream of(filename, std::ofstream::out | std::ofstream::trunc);
		of << jsonText;
		of.close();
	}

	//Saves All Scene objects to a file
	void Serialiser::SaveLevelJSON(std::vector<ECS::BaseComponent&> components, std::vector<ECS::EntityID> entities)
	{

		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

		//write enties to file
		writer.StartObject();
		writer.Key("Entities");
		writer.StartArray();

		for (auto& id:entities)
		{
			writer.Uint(id);
		}

		writer.EndArray();
		writer.EndObject();
		WriteToFileJSON("Entitys.json", buffer.GetString());
		
		//write components to file
		WriteToFileJSON("Components.json", buffer.GetString());
		//writer.StartObject();
		//for (auto& component:components)
		//{
		//	writer.StartObject();
		//	writer.EndObject();
		//	writer.a
		//	component.Serialise();
		//}
		//writer.EndObject();
	}
}