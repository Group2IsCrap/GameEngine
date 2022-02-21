#include "PlayerSystem.h"

#include <Source/Input/GetInput.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/Engine.h>

#include "../Player/PlayerComponent.h"
#include "../Player/PlayerEntity.h"

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

	if (Firelight::Input::InputGet.KeyIsPress('W'))
	{
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.y += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed);
	}

	if (Firelight::Input::InputGet.KeyIsPress('S'))
	{
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.y -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed);
	}

	if (Firelight::Input::InputGet.KeyIsPress('A'))
	{
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.x -= static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed);
	}

	if (Firelight::Input::InputGet.KeyIsPress('D'))
	{
		m_entities[0]->GetComponent<Firelight::ECS::TransformComponent>()->position.x += static_cast<float>(Firelight::Engine::Instance().GetTime().GetDeltaTime() * m_entities[0]->GetComponent<PlayerComponent>()->speed);
	}

	if (Firelight::Input::InputGet.KeyIsPress('K'))
	{
		playerEntity = new PlayerEntity(m_entities[0]->GetEntityID());
		playerEntity->RemoveHealth(1);
	}
}