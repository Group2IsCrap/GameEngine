#include "Inventory.h"
#include "Source/Graphics/AssetManager.h"
#include"Source/ECS/Components/ItemComponents.h"
Inventory::Inventory()
{
}

Inventory::Inventory(std::string name):
	m_Name(name)
{
}

Inventory::~Inventory()
{
	for (auto& Slot : Grid)
	{
		delete Slot.second;
	}
	Grid.clear();
	ECS::EntityComponentSystem::Instance()->RemoveEntity(m_inventorySpace->GetEntityID());
}
void Inventory::CreateInventoryNoPanel(Maths::Vec2f size, float slotCount, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f OffSet)
{
	m_inventorySpace = new ECS::UIPanel();
	m_inventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/Heart.png");
	m_inventorySpace->GetSpriteComponent()->toDraw = false;
	m_inventorySpace->SetAnchorSettings(Anchor);
	m_inventorySpace->SetParent(parent);
	m_inventorySpace->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	m_inventorySpace->SetOffset(OffSet);
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
	
	slotCount = slotCount;

	//number of slots per row
	float numberX = size.x / slotCount;
	if (numberX == slotCount) {
		numberX = 1.0f;
	}
	//number of rows
	float numY = slotCount / numberX;

	
	int currentPos = 0;

	SlotInfo slot;
	SlotData* slotData;
	for (size_t i = 0; i < numY; i++)
	{
		for (size_t i = 0; i < numberX; i++)
		{
			slotData = new SlotData();
			slotData->CurrSlot = &slot;
			slot.CurrPos = currentPos;

			Grid.push_back(std::make_pair(slot, slotData));
			currentPos++;
		}
		
	}

	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}
void Inventory::CreateInventoryNoPanel(Maths::Vec2f size, Maths::Vec2f rows, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f OffSet)
{
	m_inventorySpace = new ECS::UIPanel("Inventory Space");
	m_inventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/Heart.png");
	m_inventorySpace->GetSpriteComponent()->toDraw = false;
	m_inventorySpace->SetAnchorSettings(Anchor);
	m_inventorySpace->SetParent(parent);
	m_inventorySpace->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	m_inventorySpace->SetOffset(OffSet);
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

	rowCount = rows.y;
	columnCount = rows.x;
	slotCount = rows.x* rows.y;

	int currentPos = 0;
	
	SlotInfo slot;
	SlotData* slotData;
	for (size_t i = 0; i < rows.y; i++)
	{
		for (size_t i = 0; i < rows.x; i++)
		{
			slotData = new SlotData();
			slotData->CurrSlot = &slot;
			slot.CurrPos = currentPos;
			Grid.push_back(std::make_pair(slot, slotData));
			currentPos++;
		}
	}
}

//draw on screen
void Inventory::LoadInventory(std::vector<ECS::UIPanel*> *PannleToUse, bool ToFit)
{
	if (isDisplay) {
		return;
	}
	isDisplay = true;
	m_inventorySpace->GetSpriteComponent()->toDraw = isDisplay;
	


	
	if (ToFit) {
		//number of slots per row
		float columnCount = m_inventorySpace->GetWidgetComponent()->defaultDimensions.x / slotCount;
		if (columnCount == slotCount) {
			columnCount = 1.0f;
		}
		//number of rows
		float rowCount = slotCount / columnCount;
	}
	float sizeY = m_inventorySpace->GetWidgetComponent()->defaultDimensions.y / rowCount;
	float sizeX = m_inventorySpace->GetWidgetComponent()->defaultDimensions.x / columnCount;

	

	float currX = 0;
	float currY = 0;
	int currentPos = 0;
	int nextFreePannle = -1;
	for (size_t i = 0; i < PannleToUse->size(); i++)
	{
		if (!PannleToUse->at(i)->GetSpriteComponent()->toDraw) {
			nextFreePannle = i;
			break;
		}

	}

	SlotInfo slot;
	SlotData* slotData;
	for (size_t i = 0; i < rowCount; i++)
	{
		for (size_t i = 0; i < columnCount; i++)
		{
			ECS::UIPanel* Slot;
			//panel to use
			if (PannleToUse->size() > nextFreePannle && nextFreePannle != -1){
				//use exsting pannle
				Slot = PannleToUse->at(nextFreePannle);
				Slot->GetSpriteComponent()->texture=Graphics::AssetManager::Instance().GetTexture("Sprites/UI/Heart.png");
				Slot->GetSpriteComponent()->toDraw = isDisplay;
				Slot->SetParent(m_inventorySpace->GetEntityID());
				Slot->SetDefaultDimensions(Maths::Vec3f(sizeX / m_inventorySpace->GetWidgetComponent()->currentScale.x, sizeY / m_inventorySpace->GetWidgetComponent()->currentScale.y, 0));
				Slot->SetOffset(Maths::Vec2f(currX, currY));

				//find next non drawn pannle
				for (size_t i = 0; i < PannleToUse->size(); i++)
				{
					if (!PannleToUse->at(i)->GetSpriteComponent()->toDraw) {
						nextFreePannle = i;
						break;
					}

				}

			}
			else
			{
				//new pannle
				Slot = new ECS::UIPanel();
				Slot->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/UI/Heart.png");
				Slot->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
				Slot->SetParent(m_inventorySpace->GetEntityID());
				Slot->SetDefaultDimensions(Maths::Vec3f(sizeX / m_inventorySpace->GetWidgetComponent()->currentScale.x, sizeY / m_inventorySpace->GetWidgetComponent()->currentScale.y, 0));
				Slot->SetOffset(Maths::Vec2f(currX, currY));
				PannleToUse->push_back(Slot);
			}
			currX += sizeX;

			Grid[currentPos].first.slotID = Slot->GetEntityID();
			currentPos++;
		}
		currY += sizeY;
		currX = 0;
	}
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

	for (int i = 0; i < Grid.size(); i++)
	{
		if (Grid[i].second->stackSize > -1) {
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Grid[i].second->UITexID)->hasParent = true;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Grid[i].second->UITexID)->toDraw = true;

			ECS::UIBaseWidgetComponent* a = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Grid[i].second->UITexID);
			ECS::UIBaseWidgetComponent* Slot = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Grid[i].first.slotID);
			a->parentID = Grid[i].first.slotID;
			a->defaultDimensions = Maths::Vec3f(
				Slot->defaultDimensions.x / Slot->defaultScale.x,
				Slot->defaultDimensions.y / Slot->defaultScale.y,
				0);
		}
	}
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}

void Inventory::UnloadInventory()
{
	if (!isDisplay) {
		return;
	}
	isDisplay = false;
	m_inventorySpace->GetSpriteComponent()->toDraw = isDisplay;
	for (auto& Slot : Grid)
	{
		if (ECS::PixelSpriteComponent* sprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.first.slotID)) {
			sprite->toDraw = false;
		}
		Slot.first.slotID = NULL;
		if (ECS::PixelSpriteComponent* pix = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.second->UITexID)) {
			pix->toDraw = isDisplay;
		}
	}

}

void OnDragChangeScaleSettings(ECS::EntityID id) {
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(id)->scaleSetting = ECS::e_Scale::Absolute;
}
bool Inventory::AddItem(Firelight::ECS::Entity* item)
{
	bool isFail = true;
	for (auto& Slot : Grid)
	{
		if (Slot.first.isUsed == true && item->GetComponent<ECS::ItemComponent>()->itemID != ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>( Slot.second->entityIDs[0])->itemID) {
			//not free slot
			continue;
		}
		
		else {
			if (Slot.second->entityIDs.size() > Slot.second->stackSize  && Slot.second->stackSize != -1)
			{	//store here		
				continue;
			}

			Slot.second->entityIDs.push_back(item->GetEntityID());

			if (!Slot.first.isUsed) {

				if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.second->UITexID)) {
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.second->UITexID)->texture= Graphics::AssetManager::Instance().GetTexture(item->GetComponent<ECS::ItemComponent>()->iconPath);
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.second->UITexID)->toDraw = isDisplay;
					if (isDisplay) {
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.second->UITexID)->hasParent = true;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.second->UITexID)->parentID = Slot.first.slotID;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.second->UITexID)->defaultDimensions = Maths::Vec3f(
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultScale.x,
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultScale.y,
							0);
						Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
					}
				}
				else
				{
					ECS::UIEntity* Immage = new ECS::UIEntity();
					Immage->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(item->GetComponent<ECS::ItemComponent>()->iconPath);
					Immage->GetSpriteComponent()->toDraw = isDisplay;
					Immage->SetAnchorSettings(ECS::e_AnchorSettings::Center);
					Immage->GetWidgetComponent()->hasParent = false;
					if (isDisplay) {
						Immage->SetParent(Slot.first.slotID);
						Immage->SetDefaultDimensions(Maths::Vec3f(
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultScale.x,
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultScale.y,
							0));
					}
					Immage->AddComponent<ECS::UIDraggableComponent>();
					Immage->GetComponent<ECS::UIDraggableComponent>()->onDropFunctions.push_back(std::bind(&Inventory::Place, this, Slot.second));
					Immage->GetComponent<ECS::UIDraggableComponent>()->onPickUpFunctions.push_back(std::bind(&OnDragChangeScaleSettings, Immage->GetEntityID()));
					Slot.second->UITexID = Immage->GetEntityID();
				}
				if (isDisplay) {
					Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
				}
				
				Slot.second->CurrSlot = &Slot.first;
				Slot.first.isUsed = true;
				Slot.second->stackSize = item->GetComponent<ECS::ItemComponent>()->stackSize - 1;
			}
			isFail = false;
			break;
		}
		
	}
	return isFail;
}

bool Inventory::AddItem(Firelight::ECS::EntityID item)
{
	bool isFail = true;
	for (auto& Slot : Grid)
	{
		if (Slot.first.isUsed == true && ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->itemID != ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(Slot.second->entityIDs[0])->itemID) {
			//not free slot
			continue;
		}

		else {
			if (Slot.second->entityIDs.size() > Slot.second->stackSize && Slot.second->stackSize != -1)
			{	//store here		
				continue;
			}

			Slot.second->entityIDs.push_back(item);

			if (!Slot.first.isUsed) {

				if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.second->UITexID)) {
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.second->UITexID)->texture = Graphics::AssetManager::Instance().GetTexture(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->iconPath);
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.second->UITexID)->toDraw = isDisplay;
					if (isDisplay) {
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.second->UITexID)->parentID=Slot.first.slotID;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.second->UITexID)->defaultDimensions=Maths::Vec3f(
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultScale.x,
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultScale.y,
							0);
						Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
					}
				}
				else
				{
					ECS::UIEntity* Immage = new ECS::UIEntity();
					Immage->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->iconPath);
					Immage->GetSpriteComponent()->toDraw = isDisplay;
					Immage->SetAnchorSettings(ECS::e_AnchorSettings::Center);
					Immage->GetWidgetComponent()->hasParent = false;
					if (isDisplay) {
						Immage->SetParent(Slot.first.slotID);
						Immage->SetDefaultDimensions(Maths::Vec3f(
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultScale.x,
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Slot.first.slotID)->defaultScale.y,
							0));
					}
					Immage->AddComponent<ECS::UIDraggableComponent>();
					Immage->GetComponent<ECS::UIDraggableComponent>()->onDropFunctions.push_back(std::bind(&Inventory::Place, this, Slot.second));
					Slot.second->UITexID = Immage->GetEntityID();
				}
				if (isDisplay) {
					Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
				}

				Slot.second->CurrSlot = &Slot.first;
				Slot.first.isUsed = true;
				Slot.second->stackSize = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->stackSize - 1;
			}
			isFail = false;
			break;
		}

	}


	return isFail;
}

bool Inventory::AddItem(SlotData* item, bool useSlotPlacement )
{
	bool isFail = true;
	ECS::EntityID a = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item->UITexID)->parentID;
	for (auto& Slot : Grid)
	{
		if (Slot.first.isUsed == true) {
			//not free slot
			if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item->UITexID)->parentID == Slot.first.slotID && useSlotPlacement) {
				AddItem(item, false);
				break;
			}
			continue;
		}
		else if(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item->UITexID)->parentID != Slot.first.slotID && useSlotPlacement)
		{
			continue;
		}
		else {
			delete Slot.second;
			Slot.second = item;
			Slot.second->CurrSlot = &Slot.first;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(Slot.second->UITexID)->onDropFunctions.push_back(std::bind(&Inventory::Place, this, Slot.second));

			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(item->UITexID)->toDraw = isDisplay;
			Slot.first.isUsed = true;
			isFail = false;
			if (isDisplay) {
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item->UITexID)->parentID = Slot.first.slotID;
			}
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item->UITexID)->scaleSetting = ECS::e_Scale::Relative;
			break;
		}

	}
	return isFail;
}

void Inventory::RemoveItem(Firelight::ECS::Entity* item)
{
	RemoveItem(item->GetEntityID());
}

void Inventory::RemoveItem(Firelight::ECS::EntityID item)
{
	for (auto& Slot : Grid)
	{
		if (Slot.first.isUsed) {

			std::vector<ECS::EntityID>::iterator iter = Slot.second->entityIDs.begin();;
			while (iter != Slot.second->entityIDs.end()) {
				if (*iter == item) {
					iter = Slot.second->entityIDs.erase(iter);
					
					if (Slot.second->entityIDs.size() == 0)
					{
						Slot.first.isUsed = false;
						Slot.second->stackSize = -1;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(Slot.second->UITexID)->toDraw = false;
					}
					return;
				}
				else
				{
					++iter;
				}
				
			}
		}

	}
}

void Inventory::Place(SlotData* slotData)
{
	for (auto& Slot : Grid)
	{
		if (Slot.first.slotID == ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID) {

			SlotData* Temp = Slot.second;

			//swap data
			Slot.second = slotData;
			Grid[slotData->CurrSlot->CurrPos].second = Temp;

			//swap slots
			Temp->CurrSlot = &Grid[slotData->CurrSlot->CurrPos].first;
			slotData->CurrSlot = &Slot.first;

			//set data
			Slot.first.isUsed = true;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID = Slot.first.slotID;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
			if (Temp->stackSize > -1) {
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(Temp->UITexID)->parentID = Temp->CurrSlot->slotID;
				
				Temp->CurrSlot->isUsed = true;
			}
			else
			{
				Temp->CurrSlot->isUsed = false;
			}
			
			return;

		}

	}
	//not in inventory
	//remove from inventory 
	slotData->CurrSlot->isUsed = false;
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = false;

	Grid[slotData->CurrSlot->CurrPos].second = new SlotData();
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropFunctions.clear();
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Absolute;

	//to be used some were else
	m_outOfInventoryData.push_back(slotData);

	Events::EventDispatcher::InvokeFunctions<Events::Inventory::UpdateInventory>();

}

bool Inventory::FindItem(Firelight::ECS::Entity* item)
{
	return FindItem(item->GetEntityID());
}

bool Inventory::FindItem(Firelight::ECS::EntityID item)
{
	for (auto& Slot : Grid)
	{
		if (!Slot.first.isUsed) {
			continue;
		}
		for (auto& Items : Slot.second->entityIDs)
		{
			if (Items == item) {
				return true;
			}
		}	
	}
	return false;
}

bool Inventory::RemoveItemType(int howMany, int type)
{
	std::vector<std::pair<SlotInfo, SlotData*>> DataToRemove;
	int countToRemove = howMany;
	for (auto& Slot : Grid)
	{
		if (!Slot.first.isUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(Slot.second->entityIDs[0])->itemID != type) {
			continue;
		}

		countToRemove -= Slot.second->entityIDs.size();
		DataToRemove.push_back(Slot);
	
		if (countToRemove <= 0) {
			for (size_t i = 0; i < DataToRemove.size(); i++)
			{
				for (size_t j = 0; j < howMany; j++, howMany--)
				{
					DataToRemove[i].second->entityIDs.erase(DataToRemove[i].second->entityIDs.begin()+ (DataToRemove[i].second->entityIDs.size()-1));

					if (DataToRemove[i].second->entityIDs.size() == 0)
					{
						DataToRemove[i].second->CurrSlot->isUsed = false;
						DataToRemove[i].second->stackSize = -1;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(DataToRemove[i].second->UITexID)->toDraw = false;
						break;
					}
				}
			}
			return true;
		}
	}
	return false;
}

int Inventory::GetItemTypeTotal(int type)
{
	int total = 0;
	for (auto& Slot : Grid)
	{
		if (!Slot.first.isUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(Slot.second->entityIDs[0])->itemID != type) {
			continue;
		}
		total++;
	}
	return total;
}

std::vector<ECS::EntityID> Inventory::GetItemType(int howMany, int type)
{
	std::vector<std::pair<SlotInfo, SlotData*>> DataToAdd;
	std::vector<ECS::EntityID> ids;
	int countToAdd = howMany;
	for (auto& Slot : Grid)
	{
		if (!Slot.first.isUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(Slot.second->entityIDs[0])->itemID != type) {
			continue;
		}

		countToAdd -= Slot.second->entityIDs.size();
		DataToAdd.push_back(Slot);

		if (countToAdd <= 0) {
			for (size_t i = 0; i < DataToAdd.size(); i++)
			{
				for (size_t j = 0; j < howMany; j++, howMany--)
				{
					ids.push_back(DataToAdd[i].second->entityIDs[j]);
					if (howMany == 0)
					{
						return ids;
					}
				}
			}
		}
	}
	return ids;
}
