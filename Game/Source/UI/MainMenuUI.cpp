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
	borderPanel->SetDefaultDimensions(Firelight::Maths::Vec3f(800, 500, 0));
	borderPanel->SetOffset(Firelight::Maths::Vec2f(0, 200));
	m_UIEntities.push_back(borderPanel);

	Firelight::ECS::UIButton* playButton = new Firelight::ECS::UIButton("Play Button");
	playButton->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	playButton->GetSpriteComponent()->toDraw = true;
	playButton->SetParent(borderPanel->GetEntityID());
	playButton->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	playButton->SetScaleSettings(Firelight::ECS::e_Scale::Absolute);
	playButton->SetDefaultDimensions(Firelight::Maths::Vec3f(700, 100, 0));
	playButton->SetOffset(Firelight::Maths::Vec2f(0, -150));
	playButton->GetComponent<Firelight::ECS::UIPressableComponent>()->onLeftPressFunctions.push_back(std::bind(&MainMenuUI::PlayFunction, this));
	Firelight::ECS::TextComponent* text = playButton->AddComponent<Firelight::ECS::TextComponent>();
	text->text.SetString("Play");	
	text->text.SetTextHeight(100.0f);
	text->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	text->layer = 100000;
	m_UIEntities.push_back(playButton);

	//Firelight::ECS::UIButton* testUIButton2 = new Firelight::ECS::UIButton("Test UI Button 2");
	//testUIButton2->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	//testUIButton2->GetSpriteComponent()->toDraw = true;
	//testUIButton2->SetParent(testUIPanel->GetEntityID());
	//testUIButton2->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	//testUIButton->SetScaleSettings(Firelight::ECS::e_Scale::Absolute);
	//testUIButton2->SetDefaultDimensions(Firelight::Maths::Vec3f(700, 100, 0));
	//testUIButton2->SetOffset(Firelight::Maths::Vec2f(0, -200));
	//Firelight::ECS::TextComponent* text2 = testUIButton2->AddComponent<Firelight::ECS::TextComponent>();
	//text2->text.SetString("Test2");
	//text2->text.SetTextHeight(80.0f);
	//text2->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	//text2->layer = 100000;

	//Firelight::ECS::UIButton* testUIButton3 = new Firelight::ECS::UIButton("Test UI Button 3");
	//testUIButton3->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	//testUIButton3->GetSpriteComponent()->toDraw = true;
	//testUIButton3->SetParent(testUIPanel->GetEntityID());
	//testUIButton3->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	//testUIButton->SetScaleSettings(Firelight::ECS::e_Scale::Absolute);
	//testUIButton3->SetDefaultDimensions(Firelight::Maths::Vec3f(700, 100, 0));
	//testUIButton3->SetOffset(Firelight::Maths::Vec2f(0, 0));
	//Firelight::ECS::TextComponent* text3 = testUIButton3->AddComponent<Firelight::ECS::TextComponent>();
	//text3->text.SetString("Test3");
	//text3->text.SetTextHeight(80.0f);
	//text3->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	//text3->layer = 100000;

	//Firelight::ECS::UIButton* testUIButton4 = new Firelight::ECS::UIButton("Test UI Button 4");
	//testUIButton4->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	//testUIButton4->GetSpriteComponent()->toDraw = true;
	//testUIButton4->SetParent(testUIPanel->GetEntityID());
	//testUIButton4->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	//testUIButton->SetScaleSettings(Firelight::ECS::e_Scale::Absolute);
	//testUIButton4->SetDefaultDimensions(Firelight::Maths::Vec3f(700, 100, 0));
	//testUIButton4->SetOffset(Firelight::Maths::Vec2f(0, 200));
	//Firelight::ECS::TextComponent* text4 = testUIButton4->AddComponent<Firelight::ECS::TextComponent>();
	//text4->text.SetString("Test4");
	//text4->text.SetTextHeight(80.0f);
	//text4->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	//text4->layer = 100000;

	//m_UIEntities.push_back(testUIButton2);
	//m_UIEntities.push_back(testUIButton3);
	//m_UIEntities.push_back(testUIButton4);

}

void MainMenuUI::HandleEvents(const char* event, void* data)
{

}

void MainMenuUI::PlayFunction()
{
	Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::InputEvents::TogglePauseEvent>();
	Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::InputEvents::PlayGameEvent>();
	
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