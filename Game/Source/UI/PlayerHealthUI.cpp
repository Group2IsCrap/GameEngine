#include "PlayerHealthUI.h"

#include <Source/Events/Event.h>
#include <Source/Graphics/AssetManager.h>

#include "../Player/PlayerEntity.h"

PlayerHealthUI::PlayerHealthUI(Firelight::ECS::Entity* canvas)
{
	SetParent(canvas->GetEntityID());
	Firelight::Events::EventDispatcher::AddListener<Firelight::Events::PlayerHealthChanged>(this);
	GetSpriteComponent()->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	GetWidgetComponent()->anchorSettings = Firelight::ECS::e_AnchorSettings::TopLeft;
	SetDefaultDimensions(Firelight::Maths::Vec3f(400, 75, 0));
}

void PlayerHealthUI::HandleEvents(const char* event, void* data)
{
	// Change text
	int health = reinterpret_cast<int>(data);
}
