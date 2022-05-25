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
		audioComponent->soundName = "Forest Music.wav";
		break;
	case BiomeType::Swamp:
		audioComponent->soundName = "Swamp Music.wav";
		break;
	case BiomeType::Snow:
		break;
	}

	audioComponent->soundPos = biomeMusicData.playerPosition;
	this->PlayAudioClip();
}

void BackgroundMusicEntity::ChangeMusic(int biome, Vector3D playerPos)
{
	if (biome == 0)
	{
		audioComponent->soundName = "Forest Music.wav";
	}
	if (biome == 1)
	{
		audioComponent->soundName = "Swamp Music.wav";
	}

	audioComponent->soundPos = playerPos;

	this->PlayAudioClip();
}