#include "AIEntity.h"
#include "Layers.h"

AIEntity::AIEntity() : CharacterEntity()
{
	m_AIComponent = AddComponent<AIComponent>();
	m_rigidbodyComponent = GetComponent<RigidBodyComponent>();
	m_spriteComponent = GetComponent<SpriteComponent>();
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
}

AIEntity::AIEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : CharacterEntity(isTemplate, entityID)
{
	m_AIComponent = GetComponent<AIComponent>();
	m_rigidbodyComponent = GetComponent<RigidBodyComponent>();
	m_spriteComponent = GetComponent<SpriteComponent>();
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
	Firelight::ECS::EntityComponentSystem::Instance()->RemoveEntity(GetEntityID());
}

void AIEntity::FaceDirection()
{
	if (m_rigidbodyComponent == nullptr || m_spriteComponent == nullptr)
	{
		return;
	}

	Firelight::Maths::Vec3f dir = m_rigidbodyComponent->velocity;
	dir.Normalise();
	m_spriteComponent->flipX = dir.x < 0;
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
