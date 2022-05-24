#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/ECS/EntityWrappers/Entity.h>
#include <Source/ECS/Components/AudioComponents.h>

class BackgroundMusicEntity : public Firelight::ECS::Entity
{
public:
	BackgroundMusicEntity();
	BackgroundMusicEntity(std::string name);
	BackgroundMusicEntity(Firelight::ECS::EntityID entityID);
	BackgroundMusicEntity(bool isTemplate, Firelight::ECS::EntityID entityID);

	void ChangeMusic(int biome, Vector3D playerPos);


	Firelight::ECS::AudioComponent* audioComponent;
};