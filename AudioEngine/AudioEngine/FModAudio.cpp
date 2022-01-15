#include "FModAudio.h"

//Instance functions
FModAudio::Instance::Instance()
{
	//Create and initialise the system
	fModStudioSystem = NULL;
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