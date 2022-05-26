#include "BackgroundMusicEntity.h"
#include "../PCG/BiomeInfo.h"
#include "../PCG/PCGEvents.h"
#include "../Source/Events/EventDispatcher.h"

using namespace Firelight::ECS;

BackgroundMusicEntity::BackgroundMusicEntity()
{
	audioComponent = new AudioComponent();
	AddComponent<AudioComponent>(audioComponent);
	audioComponent->looping = true;
	audioComponent->is3d = false;
	audioComponent->streaming = true;
	audioComponent->channel = "Background";
	audioComponent->soundName = "Forest Music.wav";
	this->PlayAudioClip();
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::PCGEvents::OnPlayerCrossBridge>(this);
}

BackgroundMusicEntity::BackgroundMusicEntity(std::string name) : BackgroundMusicEntity()
{
}

BackgroundMusicEntity::BackgroundMusicEntity(Firelight::ECS::EntityID entityID)
{
}

BackgroundMusicEntity::BackgroundMusicEntity(bool isTemplate, Firelight::ECS::EntityID entityID)
{
}

void BackgroundMusicEntity::HandleEvents(DescriptorType event, void* data)
{
	
	BiomeMusicData biomeMusicData = *(reinterpret_cast<BiomeMusicData*>(data));

	switch (biomeMusicData.biome)
	{
	case BiomeType::Forest:
		if (audioComponent->soundName == "Forest Music.wav")
		{
			isSame = true;
		}
		else
		{
			audioComponent->soundName = "Forest Music.wav";
			isSame = false;
		}
		break;
	case BiomeType::Swamp:
		if (audioComponent->soundName == "Swamp Music.wav")
		{
			isSame = true;
		}
		else
		{
			audioComponent->soundName = "Swamp Music.wav";
			isSame = false;
		}
		break;
	case BiomeType::Snow:
		if (audioComponent->soundName == "Snow Music.wav")
		{
			isSame = true;
		}
		else
		{
			audioComponent->soundName = "Snow Music.wav";
			isSame = false;
		}
		break;
	}

	if (!isSame)
	{
		this->StopAudio();

		audioComponent->soundPos = biomeMusicData.playerPosition;
		this->PlayAudioClip();
	}
}