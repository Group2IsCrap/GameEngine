#pragma once

#include "../ECSDefines.h"
#include "../Source/Audio/AudioClip.h"

class AudioComponent : public Firelight::ECS::BaseComponent
{
	Firelight::Audio::AudioClip* audioClip;
	// Channels
};
