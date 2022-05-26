#pragma once

#ifndef _FModAudio_H_
#define _FModAudio_H_

#include "FMod/fmod.hpp"
#include "Vector2D.h"
#include "Vector3D.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <map>

#endif

namespace snowFallAudio
{
	namespace FModAudio
	{
		struct AudioChannel
		{
			AudioChannel();
			AudioChannel(std::string name, int priority, float volume);
			~AudioChannel();

			int channelPriority = 0;
			float channelVol = 0.5f;
		};

		struct Instance
		{
			Instance();
			~Instance();

			void Update();

			FMOD::System* fmodSystem;

			int m_nextChannelId;

			typedef std::map<std::string, FMOD::Sound*> soundMap;
			typedef std::map<int, FMOD::Channel*> channelMap;
			typedef std::map<FMOD::Channel*, float> volumeMap;
			typedef std::map<std::string, AudioChannel*> channelList;
			typedef std::map<FMOD::Channel*, AudioChannel> listOfChannels;

			soundMap m_sounds;
			channelMap m_channels;
			volumeMap m_volumes;
			channelList m_channelList;
			listOfChannels m_listOfChannels;
			
		};

		class AudioEngine
		{
		public:
			static AudioEngine* engine;

			AudioEngine();
			~AudioEngine();

			static void Initialise();
			static void Update();
			static void Shutdown();
			static int ErrorCheck(FMOD_RESULT result);

			void LoadSound(const std::string& soundName, bool b3d = true, bool bLooping = false, bool bStream = false);
			void UnLoadSound(const std::string& soundName);
			int PlayfModSound(const std::string& soundName, const Vector3D& soundPos, AudioChannel audioChannel, bool looping, bool is3d, bool streaming);
			void StopChannel(int channelId);
			void StopAllChannels();
			void SetChannelPos(int channelId, const Vector3D& channelPos);
			void SetChannelVolume(int channelId, float volume);
			void UnLoadAllSounds();
			void VolumeChange(float volume);

			void Ducking();

			float dBToVolume(float dB);
			float VolumeTodB(float volume);

			AudioChannel getChannel(std::string ChannelName);

		private:
			std::string audioFolder = "Assets/AudioFolder/";
			std::string newString;

		};

		

	}
}
