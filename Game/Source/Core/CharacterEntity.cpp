#include "CharacterEntity.h"

CharacterEntity::CharacterEntity()
{
	m_HealthComponent = new HealthComponent();
	AddComponent(m_HealthComponent);
}
