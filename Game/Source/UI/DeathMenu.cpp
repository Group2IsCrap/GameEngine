#include "DeathMenu.h"

#include <Source/Events/Event.h>
#include <Source/Events/UIEvents.h>
#include "../Player/PlayerEntity.h"
#include "../Events/PlayerEvents.h"

#include <Source/Graphics/AssetManager.h>

using namespace Firelight;
using namespace Firelight::Maths;
using namespace Firelight::ECS;
using namespace Firelight::Events;

DeathMenu::DeathMenu(Firelight::ECS::Entity* canvas)
{
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::PlayerEvents::OnPlayerDiedEvent>(this);

	SetParent(canvas->GetEntityID());
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	GetSpriteComponent()->colour = Firelight::Graphics::Colour::RGBA({150, 150, 150, 255});
	SetDefaultDimensions({ 800.0f, 500.0f, 0 });

	m_respawnBtn = new Firelight::ECS::UIButton();
	m_respawnBtn->GetSpriteComponent()->toDraw = false;
	m_respawnBtn->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	m_respawnBtn->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Top);
	m_respawnBtn->SetDefaultDimensions(Maths::Vec3f(880, 120, 0));
	m_respawnBtn->SetOffset(Maths::Vec2f(0.0f, 10.0f));
	m_respawnBtn->BindOnLeftPressed(std::bind(&DeathMenu::Respawn, this));
	m_respawnBtn->SetParent(canvas->GetEntityID());

	m_text = new GameEntity();

	SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	OpenMenu(false);
}

void DeathMenu::HandleEvents(const char* event, void* data)
{
	if (event != "OnPlayerDiedEvent")
		return;

	OpenMenu(true);
}

void DeathMenu::OpenMenu(bool opened)
{
	GetSpriteComponent()->toDraw = opened;
	m_respawnBtn->GetSpriteComponent()->toDraw = opened;

	if (opened)
	{
		// TODO : Draw text on top of UI? Text should really be part of UI system so that it also scales
		m_text->AddComponent<TextComponent>(new TextComponent());
		m_text->GetComponent<TextComponent>()->text.SetString("You Died!");
		m_text->GetComponent<TextComponent>()->text.SetTextHeight(150.0f);
		m_text->GetComponent<TextComponent>()->text.SetTextAnchor(Graphics::TextAnchor::e_MidMid);
		m_text->GetComponent<TextComponent>()->layer = 100000;
		m_text->GetComponent<TransformComponent>()->position = Maths::Vec3f(640.0f, 150.0f, 0.0f);
	}
	else
	{
		if (m_text->HasComponent<TextComponent>())
			m_text->RemoveComponent<TextComponent>();
	}

	EventDispatcher::InvokeFunctions<UI::UpdateUIEvent>();
}

void DeathMenu::Respawn()
{
	OpenMenu(false);
}
