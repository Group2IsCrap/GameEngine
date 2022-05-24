#include "DeathMenu.h"

#include <Source/Events/Event.h>
#include <Source/Events/UIEvents.h>
#include "../Events/InputEvents.h"
#include "../Player/PlayerEntity.h"
#include "../Events/PlayerEvents.h"
#include "../Core/Layers.h"

#include <Source/Graphics/AssetManager.h>

using namespace Firelight;
using namespace Firelight::Maths;
using namespace Firelight::ECS;
using namespace Firelight::Events;

DeathMenu::DeathMenu(Firelight::ECS::Entity* canvas)
{
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::PlayerEvents::OnPlayerDiedEvent>(this);

	SetParent(canvas->GetEntityID());
	GetSpriteComponent()->toDraw = false;
	SetDefaultDimensions({ 1920.0f, 1080.0f, 0 });

	this->GetIDComponent()->name = "Death Menu UI";

	Firelight::ECS::UIPanel* backgroundPanel = new Firelight::ECS::UIPanel("Background Death Menu Panel");
	backgroundPanel->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetColourTexture(Firelight::Graphics::Colours::sc_black);
	backgroundPanel->GetSpriteComponent()->toDraw = true;
	backgroundPanel->GetSpriteComponent()->layer = 1000;
	backgroundPanel->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	backgroundPanel->SetParent(canvas->GetEntityID());
	backgroundPanel->SetDefaultDimensions(Firelight::Maths::Vec3f(1920, 1080, 0));
	m_UIEntities.push_back(backgroundPanel);

	Firelight::ECS::UIPanel* textPanel = new Firelight::ECS::UIPanel("Death Menu Text Panel");
	textPanel->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetColourTexture(Firelight::Graphics::Colours::sc_black);
	textPanel->GetSpriteComponent()->toDraw = true;
	textPanel->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	textPanel->SetParent(backgroundPanel->GetEntityID());
	textPanel->SetDefaultDimensions(Firelight::Maths::Vec3f(1600, 300, 0));
	textPanel->SetOffset(Firelight::Maths::Vec2f(0, -300));
	Firelight::ECS::TextComponent* youDiedText = textPanel->AddComponent<Firelight::ECS::TextComponent>();
	youDiedText->text.SetString("You Died!");
	youDiedText->text.SetTextHeight(300.0f);
	youDiedText->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	youDiedText->layer = 100000;
	m_UIEntities.push_back(textPanel);

	Firelight::ECS::UIPanel* borderPanel = new Firelight::ECS::UIPanel("Death Menu Button Panel");
	borderPanel->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	borderPanel->GetSpriteComponent()->toDraw = true;
	borderPanel->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	borderPanel->SetParent(backgroundPanel->GetEntityID());
	borderPanel->SetDefaultDimensions(Firelight::Maths::Vec3f(750, 150, 0));
	borderPanel->SetOffset(Firelight::Maths::Vec2f(0, 200));
	m_UIEntities.push_back(borderPanel);

	Firelight::ECS::UIButton* respawnButton = new Firelight::ECS::UIButton("Respawn Button");
	respawnButton->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	respawnButton->GetSpriteComponent()->toDraw = true;
	respawnButton->SetParent(borderPanel->GetEntityID());
	respawnButton->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	respawnButton->SetScaleSettings(Firelight::ECS::e_Scale::Absolute);
	respawnButton->SetDefaultDimensions(Firelight::Maths::Vec3f(700, 100, 0));
	respawnButton->SetOffset(Firelight::Maths::Vec2f(0, 0));
	respawnButton->GetComponent<Firelight::ECS::UIPressableComponent>()->onLeftPressFunctions.push_back(std::bind(&DeathMenu::Respawn, this));
	Firelight::ECS::TextComponent* respawnText = respawnButton->AddComponent<Firelight::ECS::TextComponent>();
	respawnText->text.SetString("Respawn");
	respawnText->text.SetTextHeight(100.0f);
	respawnText->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	respawnText->layer = 100000;
	m_UIEntities.push_back(respawnButton);


	//// TODO : Draw text on top of UI? Text should really be part of UI system so that it also scales
	//m_text = new GameEntity("YouDied Text");
	//TextComponent* textComponent = m_text->AddComponent<TextComponent>(new TextComponent());
	//textComponent->text.SetString("You Died!");
	//textComponent->text.SetTextHeight(150.0f);
	//textComponent->text.SetTextAnchor(Graphics::TextAnchor::e_MidMid);
	//textComponent->layer = 100000;
	//m_text->GetComponent<TransformComponent>()->SetPosition(Maths::Vec3f(640.0f, 150.0f, 0.0f));

	SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	OpenMenu(false);
}

void DeathMenu::HandleEvents(const char* event, void* data)
{
	if (event != "OnPlayerDiedEvent")
		return;

	Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::InputEvents::TogglePauseEvent>();
	OpenMenu(true);
}

void DeathMenu::OpenMenu(bool opened)
{

	for (size_t i = 0; i < m_UIEntities.size(); ++i)
	{
		m_UIEntities[i]->GetSpriteComponent()->toDraw = opened;
		Firelight::ECS::TextComponent* text = m_UIEntities[i]->GetComponent<Firelight::ECS::TextComponent>();
		if (text != nullptr)
		{
			text->hidden = !opened;
		}
	}

	EventDispatcher::InvokeFunctions<UI::UpdateUIEvent>();

	//GetSpriteComponent()->toDraw = opened;
	//m_respawnBtn->GetSpriteComponent()->toDraw = opened;

	//if (opened)
	//{
	//	// TODO : Draw text on top of UI? Text should really be part of UI system so that it also scales
	//	m_text->AddComponent<TextComponent>(new TextComponent());
	//	TextComponent* textC = m_text->GetComponent<TextComponent>();
	//	textC->text.SetString("You Died!");
	//	textC->text.SetTextHeight(150.0f);
	//	textC->text.SetTextAnchor(Graphics::TextAnchor::e_MidMid);
	//	textC->layer = 100000;
	//	m_text->GetComponent<TransformComponent>()->SetPosition(Maths::Vec3f(640.0f, 150.0f, 0.0f));
	//}
	//else
	//{
	//	if (m_text->HasComponent<TextComponent>())
	//	{
	//		m_text->RemoveComponent<TextComponent>();
	//		return;
	//	}
	//}

	//m_text->GetComponent<TextComponent>()->hidden = !opened;

}

void DeathMenu::Respawn()
{
	EventDispatcher::InvokeFunctions<Firelight::Events::InputEvents::RespawnEvent>();

	Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::InputEvents::TogglePauseEvent>();

	OpenMenu(false);
}
