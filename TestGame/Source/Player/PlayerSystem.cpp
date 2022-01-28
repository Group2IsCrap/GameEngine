#include "PlayerSystem.h"

#include "Source/Input/GetInput.h"
#include "Source/Engine.h"
#include "Source/ECS/EntityWrappers/GameEntity.h"

#include "../Components/PlayerComponent.h"
#include "PlayerEntity.h"
#include "../Components/HealthComponent.h"
#include "../Events/HealthEvent.h"

#include <Source/Events/EventDispatcher.h>

static int s_takeDamage = false;

PlayerSystem::PlayerSystem()
{
	AddWhitelistComponent<PlayerComponent>();
}

void PlayerSystem::Update(const Firelight::Utils::Time& time)
{
	if (m_entities.size() <= 0 || m_entities.size() > 1)
	{
		return;
	}
	
	PlayerComponent* playerComponent = m_entities[0]->GetComponent<PlayerComponent>();

	if (Firelight::Input::InputGet.KeyIsPress('W'))
	{
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.y += Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed;
	}

	if (Firelight::Input::InputGet.KeyIsPress('A'))
	{
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.x -= Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed;
	}

	if (Firelight::Input::InputGet.KeyIsPress('S'))
	{
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.y -= Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed;
	}

	if (Firelight::Input::InputGet.KeyIsPress('D'))
	{
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.x += Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed;
	}

	if (Firelight::Input::InputGet.KeyIsPress('T'))
	{
		m_entities[0]->GetComponent<HealthComponent>()->health -= 1;
		int& health = m_entities[0]->GetComponent<HealthComponent>()->health;
		Firelight::Events::EventDispatcher::InvokeListeners<Firelight::Events::HealthEvent>((void*)health);
	}
}