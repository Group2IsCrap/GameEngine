#include "Inventory.h"
#include "Source/Graphics/AssetManager.h"
Inventory::Inventory()
{
}

Inventory::~Inventory()
{
}

void Inventory::CreatInventory(Maths::Vec2f size, float slotCount, ECS::Entity* parent)
{
	
	ECS::UIPanel* Inventory = new ECS::UIPanel();
	Inventory->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/PanelTest.png");
	Inventory->SetAnchorSettings(ECS::e_AnchorSettings::Top);
	Inventory->SetParent(parent->GetEntityID());
	Inventory->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

	//number of slots per row
	float numberX= size.x / slotCount;
	if (numberX == slotCount) {
		numberX = 1.0f;
	}
	//number of rows
	float numY = slotCount / numberX;

	float sizeY = size.y / numY;
	float sizeX = size.x / numberX;
	

	float currX = 0;
	float currY = 0;

	for (size_t i = 0; i < numY; i++)
	{
		for (size_t i = 0; i < numberX; i++)
		{
			ECS::UIPanel* Slot = new ECS::UIPanel();
			Slot->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/StopButton.png");
			Slot->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
			Slot->SetParent(Inventory->GetEntityID());
			Slot->SetDefaultDimensions(Maths::Vec3f(sizeX/ Inventory->GetWidgetComponent()->currentScale.x, sizeY/ Inventory->GetWidgetComponent()->currentScale.y, 0));
			Slot->SetOffset(Maths::Vec2f(currX, currY));
			currX += sizeX;
		}
		currY += sizeY;
		currX = 0;
	}

	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

}

void Inventory::CreatInventory(Maths::Vec2f size, Maths::Vec2f rows, ECS::Entity* parent)
{
	ECS::UIPanel* Inventory = new ECS::UIPanel();
	Inventory->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/PanelTest.png");
	Inventory->SetAnchorSettings(ECS::e_AnchorSettings::Top);
	Inventory->SetParent(parent->GetEntityID());
	Inventory->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

	

	float sizeY = size.y / rows.y;
	float sizeX = size.x / rows.x;


	float currX = 0;
	float currY = 0;

	for (size_t i = 0; i < rows.y; i++)
	{
		for (size_t i = 0; i < rows.x; i++)
		{
			ECS::UIPanel* Slot = new ECS::UIPanel();
			Slot->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/StopButton.png");
			Slot->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
			Slot->SetParent(Inventory->GetEntityID());
			Slot->SetDefaultDimensions(Maths::Vec3f(sizeX / Inventory->GetWidgetComponent()->currentScale.x, sizeY / Inventory->GetWidgetComponent()->currentScale.y, 0));
			Slot->SetOffset(Maths::Vec2f(currX, currY));
			currX += sizeX;
		}
		currY += sizeY;
		currX = 0;
	}

	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}
