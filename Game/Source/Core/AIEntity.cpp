#include "AIEntity.h"
#include "Layers.h"
#include "../Core/ResourceEntity.h"
#include "../WorldEntities/ResourceDatabase.h"

AIEntity::AIEntity() : CharacterEntity()
{
	m_AIComponent = AddComponent<AIComponent>();
	m_rigidbodyComponent = GetComponent<RigidBodyComponent>();
	m_spriteComponent = GetComponent<SpriteComponent>();
	m_transformComponent = GetComponent<TransformComponent>();
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Enemy);
	GetLayerComponent()->layer = static_cast<int>(GameLayer::Enemy);
	this->GetIDComponent()->name = "AI Entity";
}

AIEntity::AIEntity(std::string name) : AIEntity()
{
	this->GetIDComponent()->name = name;
}

AIEntity::AIEntity(Firelight::ECS::EntityID entityID) : CharacterEntity(entityID)
{
	m_AIComponent = GetComponent<AIComponent>();
	m_rigidbodyComponent = GetComponent<RigidBodyComponent>();
	m_spriteComponent = GetComponent<SpriteComponent>();
	m_transformComponent = GetComponent<TransformComponent>();
}

AIEntity::AIEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : CharacterEntity(isTemplate, entityID)
{
	m_AIComponent = GetComponent<AIComponent>();
	m_rigidbodyComponent = GetComponent<RigidBodyComponent>();
	m_spriteComponent = GetComponent<SpriteComponent>();
	m_transformComponent = GetComponent<TransformComponent>();
}

void AIEntity::RemoveHealth(int amount)
{
	// Tint red for x amount of time
	GetAIComponent()->m_tint = true;
	GetSpriteComponent()->colour = Firelight::Graphics::Colour::RGBA(200, 0, 0, 255);

	CharacterEntity::RemoveHealth(amount);
}

void AIEntity::HealthBelowZero()
{
	if (this->GetIDComponent()->name == "Deer")
	{
		this->GetComponent<AudioComponent>()->soundName = "Deer.wav";
		ResourceEntity* resourceEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(6);
		resourceEntity->GetComponent<TransformComponent>()->SetPosition(this->GetTransformComponent()->GetPosition());
		resourceEntity->RemoveHealth(1);
	}
	if (this->GetIDComponent()->name == "Crocodile")
	{
		ResourceEntity* resourceEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(5);
		resourceEntity->GetComponent<TransformComponent>()->SetPosition(this->GetTransformComponent()->GetPosition());
		resourceEntity->RemoveHealth(1);
	}
	if (this->GetIDComponent()->name == "Slime")
	{
		ResourceEntity* resourceEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(7);
		resourceEntity->GetComponent<TransformComponent>()->SetPosition(this->GetTransformComponent()->GetPosition());
		resourceEntity->RemoveHealth(1);
	}
	if (this->GetIDComponent()->name == "Bunny")
	{
		ResourceEntity* resourceEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(8);
		resourceEntity->GetComponent<TransformComponent>()->SetPosition(this->GetTransformComponent()->GetPosition());
		resourceEntity->RemoveHealth(1);
	}
	if (this->GetIDComponent()->name == "Bear")
	{
		ResourceEntity* resourceEntity = ResourceDatabase::Instance()->CreateInstanceOfResource(9);
		resourceEntity->GetComponent<TransformComponent>()->SetPosition(this->GetTransformComponent()->GetPosition());
		resourceEntity->RemoveHealth(1);
	}

	

	this->GetComponent<AudioComponent>()->soundPos = Vector3D(GetTransformComponent()->GetPosition().x, GetTransformComponent()->GetPosition().y, GetTransformComponent()->GetPosition().z);
	this->PlayAudioClip();

	GetAIComponent()->isDead = true;
}

void AIEntity::FaceDirection()
{
	if (m_rigidbodyComponent == nullptr || m_transformComponent == nullptr)
	{
		return;
	}

	Firelight::Maths::Vec3f dir = m_rigidbodyComponent->velocity;
	dir.Normalise();
	m_transformComponent->FlipX(dir.x < 0, false);
}

AIComponent* AIEntity::GetAIComponent()
{
	return m_AIComponent;
}

AIType AIEntity::GetType()
{
	return m_AIComponent->m_Type;
}

Entity* AIEntity::GetTarget()
{
	return m_AIComponent->m_Target;
}
