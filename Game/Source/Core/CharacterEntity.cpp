#include "CharacterEntity.h"

CharacterEntity::CharacterEntity()
{
	m_HealthComponent = new HealthComponent();
	AddComponent(m_HealthComponent);
}

CharacterEntity::CharacterEntity(Firelight::ECS::EntityID entityID) : Firelight::ECS::SpriteEntity(entityID)
{
}

HealthComponent* CharacterEntity::GetHealthComponent()
{
	return m_HealthComponent;
}

int CharacterEntity::GetHealth()
{
	return m_HealthComponent->currentHealth;
}

void CharacterEntity::RemoveHealth(int amount)
{
	m_HealthComponent->currentHealth -= amount;
	if (m_HealthComponent->currentHealth <= 0)
	{
		m_HealthComponent->currentHealth = 0;
		HealthBelowZero();
	}
}

void CharacterEntity::HealthBelowZero()
{
	
}
