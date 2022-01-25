#pragma once

#ifndef _FModAudio_H_
#define _FModAudio_H_

#include "Vector2D.h"
#include "Vector3D.h"
#include "FMod/fmod.hpp"
#include "FMod/fmod_studio.hpp"
#include <iostream>
#include <math.h>
#include <vector>
#include <map>

#endif

namespace FModAudio
{
	struct Instance
	{
		Instance();
		~Instance();

		void Update();

		FMOD::Studio::System* fModStudioSystem;
		FMOD::System* fmodSystem;

		int m_nextChannelId;

		typedef std::map<std::string, FMOD::Sound*> soundMap;
		typedef std::map<int, FMOD::Channel*> channelMap;

		soundMap m_sounds;
		channelMap m_channels;

	};

	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		static void Initialise();
		static void Update();
		static void Shutdown();
		static int ErrorCheck(FMOD_RESULT result);

		void LoadSound(const std::string& soundName,bool b3d = true, bool bLooping = false, bool bStream = false);
		void UnLoadSound(const std::string& soundName);
		void SetListenerPos(const Vector2D& playerPos);
		int PlaySound(const std::string& soundName, const Vector3D& soundPos = Vector3D(0.0f, 0.0f, 0.0f), float volumedB = 0.0f);
		int PlaySound2D(const std::string& soundName, const Vector2D& playerPos, float volumedB = 0.0f);
		void StopChannel(int channelId);
		void StopAllChannels();
		void SetChannelPos(int channelId, const Vector3D& channelPos);
		void SetChannelVolume(int channelId, float volumedB);
		bool IsPlaying(int channelId) const;
		void UnLoadAllSounds();

		float dBToVolume(float dB);
		float VolumeTodB(float volume);

		
	};
}