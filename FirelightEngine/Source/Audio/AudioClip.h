#pragma once

#include <string>

namespace Firelight::Audio
{
	struct AudioClip
	{
		std::string filePath;
		float volume = 1.0f;
		float pitch = 1.0f;
		bool loop = false;
		bool is3D = true;
		bool isStreamed = false;
	};
}