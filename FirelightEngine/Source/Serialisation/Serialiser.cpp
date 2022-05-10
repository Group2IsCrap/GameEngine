#include "Serialiser.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"

#include "../ECS/EntityComponentSystem.h"
#include "../Utils/ErrorManager.h"
#include <fstream>
#include <sstream>

namespace Firelight::Serialisation
{
	rapidjson::PrettyWriter<rapidjson::StringBuffer>* Serialiser::Writer = nullptr;
	rapidjson::Document* Serialiser::FileDocument = nullptr;

	void Serialiser::StartArray(std::string key)
	{
		if (!IsWriterValid())
		{
			return;
		}

		Writer->Key(key.c_str());
		Writer->StartArray();
	}

	void Serialiser::EndArray()
	{
		if (!IsWriterValid())
		{
			return;
		}

		Writer->EndArray();
	}

	void Serialiser::StartObject()
	{
		if (!IsWriterValid())
		{
			return;
		}

		Writer->StartObject();
	}

	void Serialiser::SetFormatOptions(rapidjson::PrettyFormatOptions options)
	{
		if (!IsWriterValid())
		{
			return;
		}

		Writer->SetFormatOptions(options);
	}

	bool Serialiser::LoadFile(const char* fileName)
	{
		FileDocument = new rapidjson::Document();
		std::ifstream fileObject(fileName);

		ASSERT_RETURN(fileObject, std::string("File at ") + fileName + "did not exist", false);

		std::stringstream buffer;
		buffer << fileObject.rdbuf();
		std::string fileString = buffer.str();

		// Parse the data loaded from file using rapidjson
		FileDocument->Parse(fileString.c_str());

		if (FileDocument->HasParseError())
		{
			rapidjson::ParseErrorCode error = FileDocument->GetParseError();
			int errorOffset = static_cast<int>(FileDocument->GetErrorOffset());

			int errorStart = max(0, errorOffset - 20);
			int errorEnd = min(static_cast<int>(strlen(fileName)), errorOffset + 20);

			int beforeLen = errorOffset - errorStart;
			int afterLen = errorEnd - errorOffset;

			std::string fileString = fileName;
			std::string errorString = fileString.substr(errorStart, beforeLen) + "[HERE]" + fileString.substr(errorOffset, afterLen);
			errorString.erase(std::remove(errorString.begin(), errorString.end(), '\n'), errorString.end());
			errorString.erase(std::remove(errorString.begin(), errorString.end(), '\t'), errorString.end());

			int lineNumber = Utils::StringHelpers::GetLineNumberFromOffset(fileString, errorOffset);

			std::string finalErrorString = "Parsing Scene JSON\nCode: '" + std::to_string(error) + "'\nLine Num: '" + std::to_string(lineNumber) + "'\nError String: '" + errorString + "'\n";

			ERROR_STANDARD(finalErrorString);
			return false;
		}

		return true;
	}

	void Serialiser::EndObject()
	{
		if (!IsWriterValid())
		{
			return;
		}

		Writer->EndObject();
	}

	// Creates/Overwirtes file
	void Serialiser::WriteToFileJSON(std::string filename, std::string jsonText)
	{
		std::ofstream of(filename, std::ofstream::out | std::ofstream::trunc);
		of << jsonText;
		of.close();
	}

	//Saves All Scene objects to a file
	void Serialiser::SaveSceneJSON()
	{
		rapidjson::StringBuffer buffer;
		Writer = new rapidjson::PrettyWriter<rapidjson::StringBuffer>(buffer);

		Writer->StartObject();
		Writer->String("Scene");
		Writer->StartObject();
		Firelight::ECS::EntityComponentSystem::Instance()->Serialise();
		Writer->EndObject();
		Writer->EndObject();

		WriteToFileJSON("Assets/Saves/Components.json", buffer.GetString());
		
		delete Writer;
		Writer = nullptr;
	}

	bool Serialiser::IsWriterValid()
	{
		if (Writer != nullptr)
		{
			return true;
		}
		ERROR("Failed to serialise due to missing writer.");
		return false;
	}
	
	void Serialiser::WriteKey(std::string key)
	{
		if (key != "")
		{
			Writer->Key(key.c_str());
		}
	}

	void Serialiser::Serialise(std::string name, int value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		Writer->Int(value);
	}

	void Serialiser::Serialise(std::string name, bool value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		Writer->Bool(value);
	}

	void Serialiser::Serialise(std::string name, float value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		Writer->Double((double)value);
	}

	void Serialiser::Serialise(std::string name, ECS::ComponentTypeID value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		Writer->Uint(value);
	}

	void Serialiser::Serialise(std::string name, ECS::EntityID value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		Writer->Uint(value);
	}

	void Serialiser::Serialise(std::string name, std::string value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		Writer->String(value.c_str());
	}

	void Serialiser::Serialise(std::string name, const char* value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		if (value != nullptr)
		{
			WriteKey(name);
			Writer->String(value);
		}

	}

	void Serialiser::Serialise(std::string name, Firelight::Maths::Vec3f value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		StartObject();
		WriteKey("X");
		Writer->Double(value.x);
		WriteKey("Y");
		Writer->Double(value.y);
		WriteKey("Z");
		Writer->Double(value.z);
		EndObject();
	}

	void Serialiser::Serialise(std::string name, Firelight::Maths::Vec2f value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		StartObject();
		WriteKey("X");
		Writer->Double(value.x);
		WriteKey("Y");
		Writer->Double(value.y);
		EndObject();
	}

	void Serialiser::Serialise(std::string name, Firelight::Maths::Rectf value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		StartObject();
		WriteKey("X");
		Writer->Double(value.x);
		WriteKey("Y");
		Writer->Double(value.y);
		WriteKey("Width");
		Writer->Double(value.w);
		WriteKey("Height");
		Writer->Double(value.h);
		EndObject();
	}

	void Serialiser::Serialise(std::string name, Firelight::Graphics::Texture* value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		Writer->String("INSERT TEXTURE HERE");
	}

	void Serialiser::Serialise(std::string name, Firelight::Animation::Animation* value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		Writer->String("INSERT ANIMATION HERE");
	}

	void Serialiser::Serialise(std::string name, Firelight::Graphics::Colour::RGBA value)
	{
		if (!IsWriterValid())
		{
			return;
		}

		WriteKey(name);
		StartObject();
		WriteKey("R");
		Writer->Double(value.GetR());
		WriteKey("G");
		Writer->Double(value.GetG());
		WriteKey("B");
		Writer->Double(value.GetB());
		WriteKey("A");
		Writer->Double(value.GetA());
		EndObject();
	}

	void Serialiser::Deserialize(std::string name, int& value)
	{
		if (FileDocument->HasMember(name.c_str()))
		{
			value = (*FileDocument)[name.c_str()].GetInt();
		}

	}

	void Serialiser::Deserialize(std::string name, float& value)
	{
		if (FileDocument->HasMember(name.c_str()))
		{
			value = (*FileDocument)[name.c_str()].GetFloat();
		}
	}

	void Serialiser::Deserialize(std::string name, std::string& value)
	{
		if (FileDocument->HasMember(name.c_str()))
		{
			value = (*FileDocument)[name.c_str()].GetString();
		}
	}

	
	
}