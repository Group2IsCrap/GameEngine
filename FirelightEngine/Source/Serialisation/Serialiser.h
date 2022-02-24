#pragma once
#include <string>
#include <vector>
#include "../ECS/EntityComponentSystem.h"
#include "../ECS/ECSDefines.h"
#include "../Maths/Vec3.h"
#include "../Maths/Vec2.h"
#include "../Maths/Rect.h"
#include "../Graphics/Data/Texture.h"
#include "../Graphics/Data/Colour.h"
#include "../Animation/Animation.h"

namespace Firelight::Serialisation
{
	class Serialiser
	{
	public:
		static void SaveSceneJSON();
		
		template<typename T>
		static void Serialise(std::string name, T value)
		{ 
			ERROR_STANDARD(std::string("Failed to serialise due to invalid type. " + std::string(typeid(T).name()) + " is not a serialisable type."));
		}

		template<typename T>
		static void Serialise(T value)
		{
			Serialise("", value);
		}

		static void Serialise(std::string name, int value);
		static void Serialise(std::string name, bool value);
		static void Serialise(std::string name, float value);
		static void Serialise(std::string name, ECS::ComponentTypeID value);
		static void Serialise(std::string name, ECS::EntityID value);
		static void Serialise(std::string name, std::string value);
		static void Serialise(std::string name, const char* value);
		static void Serialise(std::string name, Firelight::Maths::Vec3f value);
		static void Serialise(std::string name, Firelight::Maths::Vec2f value);
		static void Serialise(std::string name, Firelight::Maths::Rectf value);
		static void Serialise(std::string name, Firelight::Graphics::Texture* value);
		static void Serialise(std::string name, Firelight::Animation::Animation* value);
		static void Serialise(std::string name, Firelight::Graphics::Colour::RGBA value);

		static void StartArray(std::string key);
		static void EndArray();
		static void StartObject();
		static void EndObject();
		static void SetFormatOptions(rapidjson::PrettyFormatOptions options);

	private:
		static void WriteToFileJSON(std::string filename, std::string jsonText);
		static bool IsWriterValid();
		static rapidjson::PrettyWriter<rapidjson::StringBuffer>* Writer;
		static void WriteKey(std::string key);
	};
}


