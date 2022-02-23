#include "DeathMenu.h"

#include <Source/Graphics/AssetManager.h>

DeathMenu::DeathMenu(Firelight::ECS::Entity* canvas)
{
	SetParent(canvas->GetEntityID());
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	// Make menu not render

}

void DeathMenu::OpenMenu()
{
	// Set menu active here
}
