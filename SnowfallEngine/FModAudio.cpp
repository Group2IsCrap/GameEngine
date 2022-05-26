#include "FModAudio.h"
#include <io.h>
#include <Windows.h>
using namespace snowFallAudio::FModAudio;

AudioEngine* AudioEngine::engine = new AudioEngine;

AudioEngine::AudioEngine()
{
	this->Initialise();
}

AudioEngine::~AudioEngine()
{
	delete engine;
}

//Instance functions
Instance::Instance()
{
	////Create and initialise the system
	m_nextChannelId = 0;
	//fModStudioSystem = NULL;
	fmodSystem = NULL; 
	AudioEngine::engine->ErrorCheck(FMOD::System_Create(&fmodSystem));
	AudioEngine::engine->ErrorCheck(fmodSystem->init(512, FMOD_INIT_3D_RIGHTHANDED, 0));
	//AudioEngine::engine->ErrorCheck(FMOD::Studio::System::create(&fModStudioSystem));
	//AudioEngine::engine->ErrorCheck(fModStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));
	/*fModStudioSystem = NULL;
	if (AudioEngine::engine->ErrorCheck(FMOD::Studio::System::create(&fModStudioSystem)) == 2)
	{
		if (AudioEngine::engine->ErrorCheck(fModStudioSystem->initialize(512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0)) == 2)
		{
			std::cout << "No Errors" << std::endl;
		}
	}*/
}

Instance::~Instance()
{
	//Unload all assets
	//AudioEngine::engine->ErrorCheck(fModStudioSystem->unloadAll());
	//Shutdown the FMod system
	//AudioEngine::engine->ErrorCheck(fModStudioSystem->release());

	AudioEngine::engine->ErrorCheck(fmodSystem->release());

	AudioEngine::engine->Shutdown();
}

void Instance::Update()
{
	std::vector<channelMap::iterator> stoppedChannels;

	//Run through all channels
	for (auto it = m_channels.begin(), itEnd = m_channels.end(); it != itEnd; ++it)
	{
		//Check if channel is playing
		bool isChannelPlaying = false;
		it->second->isPlaying(&isChannelPlaying);
		if (!isChannelPlaying)
		{
			//Add the stopped channel to the queue
			stoppedChannels.push_back(it);
		}
	}

	for (auto& it : stoppedChannels)
	{
		//Remove stopped channels
		m_channels.erase(it);
	}
	//Call the FMOD update
	//AudioEngine::engine->Ducking();
	AudioEngine::engine->ErrorCheck(fmodSystem->update());
	//AudioEngine::engine->ErrorCheck(fModStudioSystem->update());
	
}

Instance* fmodInstance = nullptr;

//End Of Instance Functions


AudioChannel::AudioChannel()
{

}

AudioChannel::AudioChannel(std::string name, int priority, float volume)
{
	this->channelPriority = priority;
	this->channelVol = volume;

	fmodInstance->m_channelList[name] = this;
}

AudioChannel::~AudioChannel()
{

}

//Just conversion functions
float AudioEngine::dBToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float AudioEngine::VolumeTodB(float Volume)
{
	return 20.0f * log10f(Volume);
}

void AudioEngine::Initialise()
{
	//create new instance
	fmodInstance = new Instance;
	//Start update chain
	AudioChannel* UIchannel = new AudioChannel("UI", 1, 0.05f);
	AudioChannel* GameplayChannel = new AudioChannel("Game", 2, 0.025f);
	AudioChannel* PlayerChannel = new AudioChannel("Player", 3, 0.03f);
	AudioChannel* AmbienceChannel = new AudioChannel("Ambience", 2, 0.015f);
	AudioChannel* backgroundMusicChannel = new AudioChannel("Background", 2, 0.005f);
	AudioChannel* enemiesChannel = new AudioChannel("Enemies", 3, 0.025f);
	AudioEngine::engine->Update();
}

void AudioEngine::Update()
{
	//update the current instance
	fmodInstance->Update();
}

void AudioEngine::LoadSound(const std::string& soundName, bool b3d, bool bLooping, bool bStream)
{
	//Find if the sound is already loaded or not
	auto soundFound = fmodInstance->m_sounds.find(soundName);
	if (soundFound != fmodInstance->m_sounds.end())
	{
		return;
	}

	//Holds definition of params
	FMOD_MODE fmodMode = FMOD_DEFAULT;

	//Set Loading params
	fmodMode |= b3d ? FMOD_3D : FMOD_2D;
	fmodMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	fmodMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	//Load the sound with the params
	FMOD::Sound* sound = nullptr;
	engine->ErrorCheck(fmodInstance->fmodSystem->createSound(soundName.c_str(), fmodMode, nullptr, &sound));

	//Add the sound to loaded sounds
	if (sound)
	{
		fmodInstance->m_sounds[soundName] = sound;
	}
}

void AudioEngine::UnLoadSound(const std::string& soundName)
{
	//Find if the sound isn't loaded
	newString = audioFolder + soundName;
	auto soundFound = fmodInstance->m_sounds.find(newString);
	if (soundFound == fmodInstance->m_sounds.end())
	{
		return;
	}

	//Release the audio file
	engine->ErrorCheck(soundFound->second->release());
	fmodInstance->m_sounds.erase(soundFound);
}

int AudioEngine::PlayfModSound(const std::string& soundName, const Vector3D& soundPos, AudioChannel audioChannel, bool looping, bool is3d, bool streaming)
{
	newString = audioFolder + soundName;
	//Set the next channel
	int nextChannelId = fmodInstance->m_nextChannelId++;

	//Find if sound is not loaded
	auto soundFound = fmodInstance->m_sounds.find(newString);
	if (soundFound == fmodInstance->m_sounds.end())
	{
		engine->LoadSound(newString, is3d, looping, streaming);
		//Find the newly loaded sound
		soundFound = fmodInstance->m_sounds.find(newString);
		//If still not loaded
		if (soundFound == fmodInstance->m_sounds.end())
		{
			return nextChannelId;
		}
	}

	//Sound is loaded/found, define a new channel
	FMOD::Channel* channel = nullptr;
	//Define  sound, channel group ,whether paused and channel. Defined paused as true at first otherwise it could create sound control issues/mixing issues.
	engine->ErrorCheck(fmodInstance->fmodSystem->playSound(soundFound->second, nullptr, true, &channel));

	
	//if channel now exists (after assigning the sound to play)
	if (channel)
	{
		//Holder for params
		FMOD_MODE currentMode;
		soundFound->second->getMode(&currentMode);
		//if holding 3D parameter
		if (currentMode & FMOD_3D)
		{
			FMOD_VECTOR position;
			position.x = soundPos.x;
			position.y = soundPos.y;
			position.z = soundPos.z;


			//use position
			engine->ErrorCheck(channel->set3DAttributes(&position, NULL));
		}
		/*if (currentMode & FMOD_LOOP_NORMAL)
		{
			engine->ErrorCheck(channel->setMode(FMOD_LOOP_NORMAL));
		}*/

		//Ducking();
		int ok = engine->ErrorCheck(channel->setVolume(audioChannel.channelVol));
		//SetChannelVolume(nextChannelId, audioChannel.channelVol);

		/*float volume = 0.0f;

		engine->ErrorCheck(channel->getVolume(&volume));*/

		//unpause
		engine->ErrorCheck(channel->setPaused(false));

		//add the previous channel
		fmodInstance->m_volumes[channel] = VolumeTodB(audioChannel.channelVol);
		fmodInstance->m_channels[nextChannelId] = channel;
	}
	return nextChannelId;
}

void AudioEngine::SetChannelPos(int channelId, const Vector3D& channelPos)
{
	//find the channel
	auto channelFound = fmodInstance->m_channels.find(channelId);
	if (channelFound == fmodInstance->m_channels.end())
	{
		//if no channels return
		return;
	}

	//Set position
	FMOD_VECTOR position;
	position.x = channelPos.x;
	position.y = channelPos.y;
	position.z = channelPos.z;


	//use position
	engine->ErrorCheck(channelFound->second->set3DAttributes(&position, NULL));
}

void AudioEngine::SetChannelVolume(int channelId, float volume)
{
	//Find channel
	auto channelFound = fmodInstance->m_channels.find(channelId);
	if (channelFound == fmodInstance->m_channels.end())
	{
		//return if no channel
		return;
	}

	//Set channel volume
	engine->ErrorCheck(channelFound->second->setVolume(VolumeTodB(volume)));
}

void AudioEngine::VolumeChange(float volume)
{
	for (auto channel : fmodInstance->m_volumes)
	{
		float newVol;
		float currentVol = dBToVolume(channel.second);
		if (currentVol < 100 && currentVol > 0)
		{
			if (currentVol > 95)
			{
				newVol = 100;
			}
			else if (currentVol < 5)
			{
				newVol = 0;
			}
			else
			{
				newVol = currentVol + volume;
			}

			channel.second = VolumeTodB(newVol);
			channel.first->setVolume(VolumeTodB(newVol));
		}
	}
}

//Error checking function
int AudioEngine::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD ERROR " << result << std::endl;

		return 1;
	}

	std::cout << "FMOD no errors" << std::endl;
	return 2;
}

void AudioEngine::Shutdown()
{
	engine->UnLoadAllSounds();
	delete fmodInstance;
}

void AudioEngine::UnLoadAllSounds()
{
	for (auto sound : fmodInstance->m_sounds)
	{
		std::string soundName = sound.first;
		engine->UnLoadSound(soundName);
	}
}

void AudioEngine::Ducking()
{
	std::map<int, FMOD::Channel*> channelByPriority;
	int oneCount = 0;
	int twoCount = 0;
	int threeCount = 0;
	for (auto it : fmodInstance->m_listOfChannels)
	{
		channelByPriority.insert(std::pair<int, FMOD::Channel*>(it.second.channelPriority, it.first));
		switch (it.second.channelPriority)
		{
		case 1:
			oneCount++;
			break;
		case 2:
			twoCount++;
			break;
		case 3:
			threeCount++;
			break;
		}
	}
	if (oneCount > 0)
	{
		if (twoCount > 0)
		{
			if (threeCount > 0)
			{
				for (auto it : channelByPriority)
				{
					switch (it.first)
					{
					case 1:
						it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol);
						break;
					case 2:
						it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol * 0.8);
						break;
					case 3:
						it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol * 0.6);
						break;
					}
				}
			}
			else
			{
				for (auto it : channelByPriority)
				{
					switch (it.first)
					{
					case 1:
						it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol);
						break;
					case 2:
						it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol * 0.8);
						break;
					}
				}
			}

			
		}
		else if (threeCount > 0)
		{
			for (auto it : channelByPriority)
			{
				switch (it.first)
				{
				case 1:
					it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol);
					break;
				case 3:
					it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol * 0.8);
					break;
				}
			}
		}
		for (auto it : channelByPriority)
		{
			switch (it.first)
			{
			case 2:
				it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol);
				break;

			}
		}
	}
	else if (twoCount > 0)
	{
		if (threeCount > 0)
		{
			for (auto it : channelByPriority)
			{
				switch (it.first)
				{
				case 2:
					it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol);
					break;
				case 3:
					it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol * 0.8);
					break;
				}
			}
		}
		else
		{
			for (auto it : channelByPriority)
			{
				switch (it.first)
				{
				case 2:
					it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol);
					break;

				}
			}
		}
	}
	else
	{
		for (auto it : channelByPriority)
		{
			it.second->setVolume(fmodInstance->m_listOfChannels[it.second].channelVol);
		}
	}
}

void AudioEngine::StopChannel(int channelId)
{
	auto channelFound = fmodInstance->m_channels.find(channelId);
	if (channelFound != fmodInstance->m_channels.end())
	{
		channelFound->second->stop();
		return;
	}
}

void AudioEngine::StopAllChannels()
{
	for (auto channel : fmodInstance->m_channels)
	{
		StopChannel(channel.first);
	}
}

AudioChannel AudioEngine::getChannel(std::string ChannelName)
{
	return *fmodInstance->m_channelList[ChannelName];
}