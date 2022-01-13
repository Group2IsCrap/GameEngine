#pragma once
#include "Vector2D.h"
#include <iostream>
#include <math.h>


namespace FModAudio
{
	class AudioEngine
	{
		static void Initialise();
		static void Update();
		static void Shutdown();

		void LoadSound();
		void UnLoadSound();
		void SetListenerPos(const Vector2D& playerPos);
		int PlaySound(const std::string& soundName, const Vector2D& soundPos = Vector2D(0.0f, 0.0f), float volumedB = 0.0f);
		int PlaySound2D(const std::string& soundName, const Vector2D& playerPos, float volumedB = 0.0f);
		void StopChannel(int channelId);
		void StopAllChannels();
		void SetChannelPos(int channelId, const Vector2D& channelPos);
		void SetChannelVolume(int channelId, float volumedB);
		bool IsPlaying(int channelId) const;

		float dBToVolume(float dB);
		float VolumeTodB(float volume);
	};
}