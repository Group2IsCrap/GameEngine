#pragma once

#include <Source/ECS/ECSDefines.h>
#include <Source/ECS/EntityWrappers/Entity.h>
#include <Source/ECS/Components/AudioComponents.h>

using DescriptorType = const char*;

class BackgroundMusicEntity : public Firelight::ECS::Entity
							, public Firelight::Events::Listener
{
public:
	BackgroundMusicEntity();
	BackgroundMusicEntity(std::string name);
	BackgroundMusicEntity(Firelight::ECS::EntityID entityID);
	BackgroundMusicEntity(bool isTemplate, Firelight::ECS::EntityID entityID);

	virtual void HandleEvents(DescriptorType event, void* data) override;

	Firelight::ECS::AudioComponent* audioComponent;
private:
	bool isSame = false;
};
