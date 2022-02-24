#include "PlayerSystem.h"

#include "Source/Input/GetInput.h"
#include "Source/Engine.h"
#include "Source/ECS/EntityWrappers/GameEntity.h"

#include "../Components/PlayerComponent.h"
#include "PlayerEntity.h"
#include "../Components/HealthComponent.h"
#include "../Events/HealthEvent.h"

#include <Source/Events/EventDispatcher.h>
#include <Source/Animation/Animation.h>
#include <Source/ECS/Systems/AnimationSystem.h>

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

	bool keyPress = false;
	if (Firelight::Input::InputGet.KeyIsPress('W'))
	{
		SetState(m_right ? 1 : 2);
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.y += Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed;
		keyPress = true;
	}

	if (Firelight::Input::InputGet.KeyIsPress('A'))
	{
		SetState(2);
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.x -= Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed;
		keyPress = true;
	}

	if (Firelight::Input::InputGet.KeyIsPress('S'))
	{
		SetState(m_right ? 1 : 2);
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.y -= Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed;
		keyPress = true;
	}

	if (Firelight::Input::InputGet.KeyIsPress('D'))
	{
		SetState(1);
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.x += Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed;
		keyPress = true;
	}

	if (!keyPress)
	{
		SetState(0);
	}
}

void PlayerSystem::SetState(int state)
{
	if (m_state == state)
		return;
	m_state = state;

	switch (state)
	{
	case 0:
		Firelight::ECS::AnimationSystem::Instance()->Play(m_entities[0], "PlayerIdle");
		m_right = false;
		m_left = false;
		break;
	case 1:
		if (!m_right)
		{
			Firelight::ECS::AnimationSystem::Instance()->Play(m_entities[0], "PlayerWalkRight");
			m_right = true;
			m_left = false;
		}
		break;
	case 2:
		if (!m_left)
		{
			Firelight::ECS::AnimationSystem::Instance()->Play(m_entities[0], "PlayerWalkLeft");
			m_left = true;
			m_right = false;
		}
		break;
	}
}
