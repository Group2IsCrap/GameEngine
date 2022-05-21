#pragma once

#include "../ECSDefines.h"
#include "FModAudio.h"
#include "../../Serialisation/Serialiser.h"

using namespace snowFallAudio::FModAudio;
using namespace Firelight::Serialisation;

namespace Firelight::ECS
{
	struct AudioComponent : BaseComponent
	{
		//audio information
		//const std::string& soundName, const Vector3D& soundPos, bool looping, bool is3d, bool streaming, AudioChannel channel
		std::string soundName;
		Vector3D soundPos;
		bool looping;
		bool is3d;
		bool streaming;
		std::string channel;

		void Serialise() override
		{
			//variable names
			Serialiser::Serialise("SoundName", soundName);
			//Serialiser::Serialise("SoundPos", soundPos);
			Serialiser::Serialise("Looping", looping);
			Serialiser::Serialise("Is3d", is3d);
			Serialiser::Serialise("Streaming", streaming);
			Serialiser::Serialise("Channel", channel);
		}

		AudioComponent* Clone() override
		{
			//just copy everything to clone
			AudioComponent* clone = new AudioComponent();
			clone->soundName = soundName;
			clone->soundPos = soundPos;
			clone->looping = looping;
			clone->is3d = is3d;
			clone->streaming = streaming;
			clone->channel = channel;

			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiVariable("SoundName", soundName);
			ImGuiVariable("SoundPos", std::to_string(soundPos.x) + ", " + std::to_string(soundPos.y) + ", " + std::to_string(soundPos.z));
			ImGuiVariable("Looping", looping);
			ImGuiVariable("Is3D", is3d);
			ImGuiVariable("Streaming", streaming);
			ImGuiVariable("Channel", channel);
		}
	};
}