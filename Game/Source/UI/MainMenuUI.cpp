#include "MainMenuUI.h"

#include <Source/Events/Event.h>
#include <Source/Events/EventDispatcher.h>
#include <Source/Graphics/AssetManager.h>
#include "../Core/Layers.h"
#include "../Events/GameEvents.h"
#include "../Events/InputEvents.h"

MainMenuUI::MainMenuUI(Firelight::ECS::Entity* canvas)
{
	SetParent(canvas->GetEntityID());
	GetSpriteComponent()->toDraw = false;
	SetDefaultDimensions(Firelight::Maths::Vec3f(128, 100, 0));
	this->GetIDComponent()->name = "Main Menu UI";


	Firelight::ECS::UIPanel* backgroundPanel = new Firelight::ECS::UIPanel("Background Main Menu Panel");
	backgroundPanel->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetColourTexture(Firelight::Graphics::Colours::sc_black);
	backgroundPanel->GetSpriteComponent()->toDraw = true;
	backgroundPanel->GetSpriteComponent()->layer = 1000;
	backgroundPanel->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	backgroundPanel->SetParent(canvas->GetEntityID());
	backgroundPanel->SetDefaultDimensions(Firelight::Maths::Vec3f(1920, 1080, 0));
	m_UIEntities.push_back(backgroundPanel);

	Firelight::ECS::UIPanel* titlePanel = new Firelight::ECS::UIPanel("Main Menu Title Image");
	titlePanel->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/UI/TitleScreen.png");
	titlePanel->GetSpriteComponent()->toDraw = true;
	titlePanel->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	titlePanel->SetParent(backgroundPanel->GetEntityID());
	titlePanel->SetDefaultDimensions(Firelight::Maths::Vec3f(1237, 453, 0));
	titlePanel->SetOffset(Firelight::Maths::Vec2f(0, -300));
	m_UIEntities.push_back(titlePanel);

	Firelight::ECS::UIPanel* borderPanel = new Firelight::ECS::UIPanel("Main Menu Button Panel");
	borderPanel->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	borderPanel->GetSpriteComponent()->toDraw = true;
	borderPanel->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	borderPanel->SetParent(backgroundPanel->GetEntityID());
	borderPanel->SetDefaultDimensions(Firelight::Maths::Vec3f(750, 150, 0));
	borderPanel->SetOffset(Firelight::Maths::Vec2f(0, 100));
	m_UIEntities.push_back(borderPanel);

	playButton = new Firelight::ECS::UIButton("Play Button");
	playButton->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	playButton->GetSpriteComponent()->toDraw = true;
	playButton->SetParent(borderPanel->GetEntityID());
	playButton->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	playButton->SetScaleSettings(Firelight::ECS::e_Scale::Absolute);
	playButton->SetDefaultDimensions(Firelight::Maths::Vec3f(700, 100, 0));
	playButton->GetComponent<Firelight::ECS::UIPressableComponent>()->onLeftPressFunctions.push_back(std::bind(&MainMenuUI::PlayFunction, this));
	Firelight::ECS::TextComponent* playText = playButton->AddComponent<Firelight::ECS::TextComponent>();
	playText->text.SetString("Play");	
	playText->text.SetTextHeight(110.0f);
	playText->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	playText->layer = 100000;
	m_UIEntities.push_back(playButton);

	Firelight::ECS::UIPanel* borderPanel2 = new Firelight::ECS::UIPanel("Main Menu Button Panel 2");
	borderPanel2->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	borderPanel2->GetSpriteComponent()->toDraw = true;
	borderPanel2->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	borderPanel2->SetParent(backgroundPanel->GetEntityID());
	borderPanel2->SetDefaultDimensions(Firelight::Maths::Vec3f(750, 150, 0));
	borderPanel2->SetOffset(Firelight::Maths::Vec2f(0, 300));
	m_UIEntities.push_back(borderPanel2);

	Firelight::ECS::UIButton* exitButton = new Firelight::ECS::UIButton("Exit Button");
	exitButton->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	exitButton->GetSpriteComponent()->toDraw = true;
	exitButton->SetParent(borderPanel2->GetEntityID());
	exitButton->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	exitButton->SetScaleSettings(Firelight::ECS::e_Scale::Absolute);
	exitButton->SetDefaultDimensions(Firelight::Maths::Vec3f(700, 100, 0));
	Firelight::ECS::TextComponent* exitText = exitButton->AddComponent<Firelight::ECS::TextComponent>();
	exitButton->GetComponent<Firelight::ECS::UIPressableComponent>()->onLeftPressFunctions.push_back(std::bind(&MainMenuUI::ExitFunction, this));
	exitText->text.SetString("Exit");
	exitText->text.SetTextHeight(110.0f);
	exitText->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	exitText->layer = 100000;
	m_UIEntities.push_back(exitButton);


}

void MainMenuUI::HandleEvents(const char* event, void* data)
{

}

void MainMenuUI::PlayFunction()
{
	Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::InputEvents::TogglePauseEvent>();
	Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::InputEvents::PlayGameEvent>();
	
	playButton->PlayAudioClip();

	for (size_t i = 0; i < m_UIEntities.size(); ++i)
	{
		m_UIEntities[i]->GetSpriteComponent()->toDraw = false;
		Firelight::ECS::TextComponent* text = m_UIEntities[i]->GetComponent<Firelight::ECS::TextComponent>();
		if (text != nullptr)
		{
			text->hidden = true;
		}
	}
}

void MainMenuUI::ExitFunction()
{
	exit(0);
}