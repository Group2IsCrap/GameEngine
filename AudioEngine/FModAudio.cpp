#include "FModAudio.h"

float FModAudio::AudioEngine::dBToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float FModAudio::AudioEngine::VolumeTodB(float Volume)
{
	return 20.0f * log10f(Volume);
}