#include "MainMenuUI.h"

#include <Source/Events/Event.h>
#include <Source/Graphics/AssetManager.h>
#include "../Core/Layers.h"

MainMenuUI::MainMenuUI(Firelight::ECS::Entity* canvas)
{
	SetParent(canvas->GetEntityID());
	GetSpriteComponent()->toDraw = false;
	SetDefaultDimensions(Firelight::Maths::Vec3f(128, 100, 0));
	this->GetIDComponent()->name = "Main Menu UI";

	Firelight::ECS::UIPanel* testUIPanel = new Firelight::ECS::UIPanel("Test UI Base Panel");
	testUIPanel->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	testUIPanel->GetSpriteComponent()->toDraw = true;
	testUIPanel->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	testUIPanel->SetParent(canvas->GetEntityID());
	testUIPanel->SetDefaultDimensions(Firelight::Maths::Vec3f(800, 800, 0));

	Firelight::ECS::UIButton* testUIButton = new Firelight::ECS::UIButton("Test UI Button 1");
	testUIButton->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	testUIButton->GetSpriteComponent()->toDraw = true;
	testUIButton->SetParent(testUIPanel->GetEntityID());
	testUIButton->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	testUIButton->SetScaleSettings(Firelight::ECS::e_Scale::Absolute);
	testUIButton->SetDefaultDimensions(Firelight::Maths::Vec3f(700, 100, 0));
	testUIButton->SetOffset(Firelight::Maths::Vec2f(0, -300));
	Firelight::ECS::TextComponent* text = testUIButton->AddComponent<Firelight::ECS::TextComponent>();
	text->text.SetString("Test");	
	text->text.SetTextHeight(80.0f);
	text->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	text->layer = 100000;

	Firelight::ECS::UIButton* testUIButton2 = new Firelight::ECS::UIButton("Test UI Button 2");
	testUIButton2->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	testUIButton2->GetSpriteComponent()->toDraw = true;
	testUIButton2->SetParent(testUIPanel->GetEntityID());
	testUIButton2->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	testUIButton2->SetDefaultDimensions(Firelight::Maths::Vec3f(1700, 200, 0));
	testUIButton2->SetOffset(Firelight::Maths::Vec2f(0, -200));
	Firelight::ECS::TextComponent* text2 = testUIButton2->AddComponent<Firelight::ECS::TextComponent>();
	text2->text.SetString("Test2");
	text2->text.SetTextHeight(80.0f);
	text2->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	text2->layer = 100000;

	Firelight::ECS::UIButton* testUIButton3 = new Firelight::ECS::UIButton("Test UI Button 3");
	testUIButton3->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	testUIButton3->GetSpriteComponent()->toDraw = true;
	testUIButton3->SetParent(testUIPanel->GetEntityID());
	testUIButton3->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	testUIButton3->SetDefaultDimensions(Firelight::Maths::Vec3f(1700, 200, 0));
	testUIButton3->SetOffset(Firelight::Maths::Vec2f(0, 0));
	Firelight::ECS::TextComponent* text3 = testUIButton3->AddComponent<Firelight::ECS::TextComponent>();
	text3->text.SetString("Test3");
	text3->text.SetTextHeight(80.0f);
	text3->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	text3->layer = 100000;

	Firelight::ECS::UIButton* testUIButton4 = new Firelight::ECS::UIButton("Test UI Button 4");
	testUIButton4->GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/ButtonTest.png");
	testUIButton4->GetSpriteComponent()->toDraw = true;
	testUIButton4->SetParent(testUIPanel->GetEntityID());
	testUIButton4->SetAnchorSettings(Firelight::ECS::e_AnchorSettings::Center);
	testUIButton4->SetDefaultDimensions(Firelight::Maths::Vec3f(1700, 200, 0));
	testUIButton4->SetOffset(Firelight::Maths::Vec2f(0, 200));
	Firelight::ECS::TextComponent* text4 = testUIButton4->AddComponent<Firelight::ECS::TextComponent>();
	text4->text.SetString("Test4");
	text4->text.SetTextHeight(80.0f);
	text4->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_MidMid);
	text4->layer = 100000;


}

void MainMenuUI::HandleEvents(const char* event, void* data)
{

}