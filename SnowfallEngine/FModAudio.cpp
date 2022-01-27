#include "FModAudio.h"
#include <io.h>

snowFallAudio::FModAudio::AudioEngine* engine = new snowFallAudio::FModAudio::AudioEngine;

snowFallAudio::FModAudio::AudioEngine::AudioEngine()
{

}

snowFallAudio::FModAudio::AudioEngine::~AudioEngine()
{
	delete engine;
}



//Instance functions
snowFallAudio::FModAudio::Instance::Instance()
{
	//Create and initialise the system
	m_nextChannelId = 0;
	fModStudioSystem = NULL;
	fmodSystem = NULL;
	engine->ErrorCheck(FMOD::Studio::System::create(&fModStudioSystem));
	engine->ErrorCheck(fModStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));
}

snowFallAudio::FModAudio::Instance::~Instance()
{

	//Unload all assets
	engine->ErrorCheck(fModStudioSystem->unloadAll());
	//Shutdown the FMod system
	engine->ErrorCheck(fModStudioSystem->release());
	engine->ErrorCheck(fmodSystem->release());

	engine->Shutdown();
}

void snowFallAudio::FModAudio::Instance::Update()
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
	engine->ErrorCheck(fModStudioSystem->update());
}

snowFallAudio::FModAudio::Instance* fmodInstance = nullptr;

//End Of Instance Functions




//Just conversion functions
float snowFallAudio::FModAudio::AudioEngine::dBToVolume(float dB)
{
	return powf(10.0f, 0.05f * dB);
}

float snowFallAudio::FModAudio::AudioEngine::VolumeTodB(float Volume)
{
	return 20.0f * log10f(Volume);
}

void snowFallAudio::FModAudio::AudioEngine::Initialise()
{
	//create new instance
	fmodInstance = new Instance;
}

void snowFallAudio::FModAudio::AudioEngine::Update()
{
	//update the current instance
	fmodInstance->Update();
}

void snowFallAudio::FModAudio::AudioEngine::LoadSound(const std::string& soundName, bool b3d, bool bLooping, bool bStream)
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

void snowFallAudio::FModAudio::AudioEngine::UnLoadSound(const std::string& soundName)
{
	//Find if the sound isn't loaded
	auto soundFound = fmodInstance->m_sounds.find(soundName);
	if (soundFound == fmodInstance->m_sounds.end())
	{
		return;
	}

	//Release the audio file
	engine->ErrorCheck(soundFound->second->release());
	fmodInstance->m_sounds.erase(soundFound);
}

int snowFallAudio::FModAudio::AudioEngine::PlaySound(const std::string& soundName, const Vector3D& soundPos, float volumedB)
{

	//Set the next channel
	int nextChannelId = fmodInstance->m_nextChannelId++;

	//Find if song is not loaded
	auto soundFound = fmodInstance->m_sounds.find(soundName);
	if (soundFound == fmodInstance->m_sounds.end())
	{
		engine->LoadSound(soundName);
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
			//Setup the annoying FMOD_VECTOR
			FMOD_VECTOR position;
			position.x = soundPos.x;
			position.y = soundPos.y;
			position.z = soundPos.z;

			//set the 3d position
			engine->ErrorCheck(channel->set3DAttributes(&position, nullptr));
		}
		//set volume
		engine->ErrorCheck(channel->setVolume(engine->dBToVolume(volumedB)));
		//unpause
		engine->ErrorCheck(channel->setPaused(false));

		//add the previous channel
		fmodInstance->m_channels[nextChannelId] = channel;
	}
	return nextChannelId;
}

void snowFallAudio::FModAudio::AudioEngine::SetChannelPos(int channelId, const Vector3D& channelPos)
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

void snowFallAudio::FModAudio::AudioEngine::SetChannelVolume(int channelId, float volumedB)
{
	//Find channel
	auto channelFound = fmodInstance->m_channels.find(channelId);
	if (channelFound == fmodInstance->m_channels.end())
	{
		//return if no channel
		return;
	}

	//Set channel volume
	engine->ErrorCheck(channelFound->second->setVolume(dBToVolume(volumedB)));
}

//Error checking function
int snowFallAudio::FModAudio::AudioEngine::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		std::cout << "FMOD ERROR " << result << std::endl;

		return 1;
	}

	std::cout << "FMOD no errors" << std::endl;
	return 1;
}

void snowFallAudio::FModAudio::AudioEngine::Shutdown()
{
	engine->UnLoadAllSounds();
	delete fmodInstance;
}

void snowFallAudio::FModAudio::AudioEngine::UnLoadAllSounds()
{
	for (auto sound : fmodInstance->m_sounds)
	{
		std::string soundName = sound.first;
		engine->UnLoadSound(soundName);
	}
}