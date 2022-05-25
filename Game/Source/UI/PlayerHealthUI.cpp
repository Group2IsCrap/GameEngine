#include "PlayerHealthUI.h"

#include <Source/Events/Event.h>
#include "../Events/PlayerEvents.h"
#include <Source/Graphics/AssetManager.h>
#include "../Core/Layers.h"

#include "../Player/PlayerEntity.h"

PlayerHealthUI::PlayerHealthUI(Firelight::ECS::Entity* canvas, int startHealth)
{
	SetParent(canvas->GetEntityID());
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::PlayerEvents::OnPlayerHealthChangedEvent>(this);
	GetSpriteComponent()->toDraw = false;
	SetDefaultDimensions(Firelight::Maths::Vec3f(128, 100, 0));
	m_startHealth = startHealth;

	this->GetIDComponent()->name = "Player Health UI";

	for (int i = 0; i < m_startHealth; ++i)
	{
		Firelight::ECS::UIPanel* heartUIEntity = new Firelight::ECS::UIPanel("Heart UI " + std::to_string(i));
		heartUIEntity->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/UI/Heart.png");
		heartUIEntity->GetSpriteComponent()->toDraw = false;
		heartUIEntity->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::TopLeft);
		heartUIEntity->SetOffset(Firelight::Maths::Vec2f((i * 75.0f), 10.0f));
		heartUIEntity->SetParent(canvas->GetEntityID());
		heartUIEntity->SetDefaultDimensions(Firelight::Maths::Vec3f(96, 72, 0));
		m_healthUIEntities.push_back(heartUIEntity);
	}
}

void PlayerHealthUI::HandleEvents(const char* event, void* data)
{
	if (event != "OnPlayerHealthChangedEvent")
		return;

	int health = reinterpret_cast<int>(data);
	SetHealth(health);
}

void PlayerHealthUI::SetHealth(int health)
{
	for (int i = 0; i < m_startHealth; ++i)
	{
		m_healthUIEntities[i]->GetSpriteComponent()->toDraw = health == 0 ? false : health - 1 >= i;
	}
}
