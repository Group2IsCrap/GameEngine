#include "AIEntity.h"
#include "Layers.h"

AIEntity::AIEntity() : CharacterEntity()
{
	m_AIComponent = AddComponent<AIComponent>();
	GetSpriteComponent()->layer = static_cast<int>(RenderLayer::Enemy);
	GetLayerComponent()->layer = static_cast<int>(GameLayer::Enemy);
}

AIEntity::AIEntity(Firelight::ECS::EntityID entityID) : CharacterEntity(entityID)
{
	m_AIComponent = GetComponent<AIComponent>();
}

AIEntity::AIEntity(bool isTemplate, Firelight::ECS::EntityID entityID) : CharacterEntity(isTemplate, entityID)
{
	m_AIComponent = GetComponent<AIComponent>();
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
