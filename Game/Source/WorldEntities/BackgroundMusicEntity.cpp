#include "BackgroundMusicEntity.h"

using namespace Firelight::ECS;

BackgroundMusicEntity::BackgroundMusicEntity()
{
	audioComponent = new AudioComponent();
	AddComponent<AudioComponent>(audioComponent);
	audioComponent->looping = true;
	audioComponent->is3d = false;
	audioComponent->streaming = true;
	audioComponent->channel = "Background";
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