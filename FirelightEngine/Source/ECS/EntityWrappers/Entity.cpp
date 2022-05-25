#include "Entity.h"

namespace Firelight::ECS
{
	Entity::Entity()
	{
		m_entityID = EntityComponentSystem::Instance()->CreateEntity();
	}

	Entity::Entity(EntityID id)
	{
		m_entityID = EntityComponentSystem::Instance()->CreateEntity(id);
	}
	
	Entity::Entity(bool isTemplate, EntityID templateID)
	{
		m_entityID = EntityComponentSystem::Instance()->CreateEntityFromTemplate(templateID);
	}

	Entity::~Entity()
	{
	}
	void Entity::Destroy()
	{
		EntityComponentSystem::Instance()->RemoveEntity(m_entityID);
	}
	EntityID Entity::GetEntityID()
	{
		return m_entityID;
	}
	Signature Entity::GetSignature()
	{
		return EntityComponentSystem::Instance()->GetSignature(m_entityID);
	}

	void Entity::PlayAudioClip()
	{
		if (this->HasComponent<AudioComponent>())
		{
			AudioComponent* audioComponent = GetComponent<AudioComponent>();
			PlaySound_Internal(audioComponent);
		}
	}

	void Entity::StopAudio()
	{
		snowFallAudio::FModAudio::AudioEngine::engine->StopAllChannels();
	}

	void Entity::PlaySound_Internal(AudioComponent* audioComponent)
	{
		//const std::string& soundName, const Vector3D& soundPos, bool looping, bool is3d, bool streaming, AudioChannel channel
		snowFallAudio::FModAudio::AudioEngine::engine->PlayfModSound(audioComponent->soundName, audioComponent->soundPos, AudioEngine::engine->getChannel(audioComponent->channel), audioComponent->looping, audioComponent->is3d, audioComponent->streaming);
	}
}