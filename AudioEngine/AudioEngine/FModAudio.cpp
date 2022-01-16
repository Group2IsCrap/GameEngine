#include "FModAudio.h"
#include <io.h>

//Instance functions
FModAudio::Instance::Instance()
{
	//Create and initialise the system
	fModStudioSystem = NULL;
	fmodSystem = NULL;
	FModAudio::AudioEngine::ErrorCheck(FMOD::Studio::System::create(&fModStudioSystem));
	FModAudio::AudioEngine::ErrorCheck(fModStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));
}

FModAudio::Instance::~Instance()
{
	//Unload all assets
	FModAudio::AudioEngine::ErrorCheck(fModStudioSystem->unloadAll());
	//Shutdown the FMod system
	FModAudio::AudioEngine::ErrorCheck(fModStudioSystem->release());
}

void FModAudio::Instance::Update()
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
	FModAudio::AudioEngine::ErrorCheck(fModStudioSystem->update());
}

FModAudio::Instance* fmodInstance = nullptr;

//End Of Instance Functions


//Just conversion functions
float FModAudio::AudioEngine::dBToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float FModAudio::AudioEngine::VolumeTodB(float Volume)
{
	return 20.0f * log10f(Volume);
}

void FModAudio::AudioEngine::Initialise()
{
	//create new instance
	fmodInstance = new Instance;
}

void FModAudio::AudioEngine::Update()
{
	//update the current instance
	fmodInstance->Update();
}

void FModAudio::AudioEngine::LoadSound(const std::string& soundName,bool b3d, bool bLooping, bool bStream)
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
	FModAudio::AudioEngine::ErrorCheck(fmodInstance->fmodSystem->createSound(soundName.c_str(), fmodMode, nullptr, &sound));

	//Add the sound to loaded sounds
	if (sound)
	{
		fmodInstance->m_sounds[soundName] = sound;
	}
}

void FModAudio::AudioEngine::UnLoadSound(const std::string& soundName)
{
	//Find if the sound isn't loaded
	auto soundFound = fmodInstance->m_sounds.find(soundName);
	if (soundFound == fmodInstance->m_sounds.end())
	{
		return;
	}

	//Release the audio file
	FModAudio::AudioEngine::ErrorCheck(soundFound->second->release());
	fmodInstance->m_sounds.erase(soundFound);
}

int FModAudio::AudioEngine::PlaySound(const std::string& soundName, const Vector3D& soundPos, float volumedB)
{
	//Set the next channel
	int nextChannelId = fmodInstance->m_nextChannelId++;

	//Find if song is not loaded
	auto soundFound = fmodInstance->m_sounds.find(soundName);
	if (soundFound == fmodInstance->m_sounds.end())
	{
		LoadSound(soundName);
		//Find the newly loaded sound
		soundFound = fmodInstance->m_sounds.find(soundName);
		//If still not loaded
		if (soundFound == fmodInstance->m_sounds.end())
		{
			return nextChannelId;
		}
	}

	//Sound is loaded/found, define a new channel
	FMOD::Channel* channel = nullptr;
	//Define channel, sound, channel group and whether paused. Defined paused as true at first otherwise it could create sound control issues/mixing issues.
	FModAudio::AudioEngine::ErrorCheck(fmodInstance->fmodSystem->playSound(soundFound->second, nullptr, true, &channel));
	//if channel now exists (after assigning the sound to play)
	if (channel)
	{
		//Holder for params
		FMOD_MODE currentMode;
		soundFound->second->getMode(&currentMode);
		//if holding 3D parameter
		if (currentMode & FMOD_3D)
		{
			//Setup the annoying FMOD_VECTOR
			FMOD_VECTOR position;
			position.x = soundPos.x;
			position.y = soundPos.y;
			position.z = soundPos.z;

			//set the 3d position
			FModAudio::AudioEngine::ErrorCheck(channel->set3DAttributes(&position, nullptr));
		}
		//set volume
		FModAudio::AudioEngine::ErrorCheck(channel->setVolume(dBToVolume(volumedB)));
		//unpause
		FModAudio::AudioEngine::ErrorCheck(channel->setPaused(false));

		//add the previous channel
		fmodInstance->m_channels[nextChannelId] = channel;
	}
	return nextChannelId;
}

void SetChannelPos(int channelId, const Vector3D& channelPos)
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
	FModAudio::AudioEngine::ErrorCheck(channelFound->second->set3DAttributes(&position, NULL));
}

void FModAudio::AudioEngine::SetChannelVolume(int channelId, float volumedB)
{
	//Find channel
	auto channelFound = fmodInstance->m_channels.find(channelId);
	if (channelFound == fmodInstance->m_channels.end())
	{
		//return if no channel
		return;
	}

	//Set channel volume
	FModAudio::AudioEngine::ErrorCheck(channelFound->second->setVolume(dBToVolume(volumedB)));
}

//Error checking function
int FModAudio::AudioEngine::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD ERROR " << result << std::endl;

		return 1;
	}

	std::cout << "FMOD no errors" << std::endl;
	return 1;
}

void FModAudio::AudioEngine::Shutdown()
{
	delete fmodInstance;
}