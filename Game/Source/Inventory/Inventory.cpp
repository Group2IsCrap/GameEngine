#include "Inventory.h"
#include "Source/Graphics/AssetManager.h"
#include"Source/ECS/Components/ItemComponents.h"

Inventory::Inventory()
{
}

Inventory::Inventory(std::string Name)
{
}

Inventory::~Inventory()
{
	ECS::EntityComponentSystem::Instance()->RemoveEntity(m_InventorySpace->GetEntityID());
}

void Inventory::SetEntityData(ECS::EntityID ID, int InvListPos) {
	m_InventoryEntityID = ID;
	m_GroupInventoryID = InvListPos;
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	m_InventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(Iventory->BackgroundTex);
}

void Inventory::CreateInventoryNoPannel(Maths::Vec2f size, float slotCount, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f OffSet)
{
	m_InventorySpace = new ECS::UIPanel();
	m_InventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	m_InventorySpace->GetSpriteComponent()->toDraw = false;
	m_InventorySpace->SetAnchorSettings(Anchor);
	m_InventorySpace->SetParent(parent);
	m_InventorySpace->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	m_InventorySpace->SetOffset(OffSet);
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}
void Inventory::CreateInventoryNoPannel(Maths::Vec2f size, Maths::Vec2f rows, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f OffSet)
{
	m_InventorySpace = new ECS::UIPanel();
	m_InventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	m_InventorySpace->GetSpriteComponent()->toDraw = false;
	m_InventorySpace->SetAnchorSettings(Anchor);
	m_InventorySpace->SetParent(parent);
	m_InventorySpace->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	m_InventorySpace->SetOffset(OffSet);
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}

//draw on screen
void Inventory::LoadInventory(std::vector<ECS::UIPanel*> *PannleToUse, bool ToFit)
{
	InventoryComponent* inventoryData= ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);

	if (inventoryData->isDisplay) {
		return;
	}
	inventoryData->isDisplay = true;
	m_InventorySpace->GetSpriteComponent()->toDraw = inventoryData->isDisplay;

	if (ToFit) {
		//number of slots per row
		float ColoumCount = m_InventorySpace->GetWidgetComponent()->defaultDimensions.x / inventoryData->SlotCount;
		if (ColoumCount == inventoryData->SlotCount) {
			ColoumCount = 1.0f;
		}
		//number of rows
		float RowCount = inventoryData->SlotCount / ColoumCount;
	}
	float sizeY = m_InventorySpace->GetWidgetComponent()->defaultDimensions.y / inventoryData->RowCount;
	float sizeX = m_InventorySpace->GetWidgetComponent()->defaultDimensions.x / inventoryData->ColoumCount;

	

	float currX = 0;
	float currY = 0;
	int currentPos = inventoryData->SlotStartPositon;
	int nextFreePannle = -1;
	for (size_t i = 0; i < PannleToUse->size(); i++)
	{
		if (!PannleToUse->at(i)->GetSpriteComponent()->toDraw) {
			nextFreePannle = i;
			break;
		}

	}

	for (size_t i = 0; i < inventoryData->RowCount; i++)
	{
		for (size_t i = 0; i < inventoryData->ColoumCount; i++)
		{
			ECS::UIPanel* Slot;
			//panel to use
			if (PannleToUse->size() > nextFreePannle && nextFreePannle != -1){
				//use exsting pannle
				Slot = PannleToUse->at(nextFreePannle);
				Slot->GetSpriteComponent()->texture=Graphics::AssetManager::Instance().GetTexture(inventoryData->SlotTex);
				Slot->GetSpriteComponent()->toDraw = inventoryData->isDisplay;
				Slot->SetParent(m_InventorySpace->GetEntityID());
				Slot->SetDefaultDimensions(Maths::Vec3f(sizeX / m_InventorySpace->GetWidgetComponent()->currentScale.x, sizeY / m_InventorySpace->GetWidgetComponent()->currentScale.y, 0));
				Slot->SetOffset(Maths::Vec2f(currX, currY));
				Slot->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
				
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
				Slot->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(inventoryData->SlotTex);
				Slot->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
				Slot->SetParent(m_InventorySpace->GetEntityID());
				Slot->SetDefaultDimensions(Maths::Vec3f(sizeX / m_InventorySpace->GetWidgetComponent()->currentScale.x, sizeY / m_InventorySpace->GetWidgetComponent()->currentScale.y, 0));
				Slot->SetOffset(Maths::Vec2f(currX, currY));
				PannleToUse->push_back(Slot);
			}
			currX += sizeX;
			
			ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, currentPos)->SlotID = Slot->GetEntityID();
			
			currentPos++;
		}
		currY += sizeY;
		currX = 0;
	}
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		
		InventoryStoreData* data = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, data->SlotIndex);
		InventoryComponentSpecialSlot* SpecialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(m_InventoryEntityID, slot->SpecialSlotIndex);

		
		if (SpecialSlot) {
			ECS::EntityComponentSystem::Instance()->GetComponent< Firelight::ECS::UIBaseWidgetComponent >(slot->SlotID)->offSet = SpecialSlot->OffSet;
			ECS::EntityComponentSystem::Instance()->GetComponent< Firelight::ECS::UIBaseWidgetComponent >(slot->SlotID)->anchorSettings = SpecialSlot->AnchorSettings;
		}

		if (data->StackSize > -1) {
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(data->UITexID)->hasParent = true;
			ECS::PixelSpriteComponent* c =ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(data->UITexID);
			c->toDraw = true;
			
			ECS::UIBaseWidgetComponent* a = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(data->UITexID);
			ECS::UIBaseWidgetComponent* Slot = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID);
			a->parentID = slot->SlotID;
			a->defaultDimensions = Maths::Vec3f(
				Slot->defaultDimensions.x / Slot->defaultScale.x,
				Slot->defaultDimensions.y / Slot->defaultScale.y,
				0);
			a->hasParent = true;
		}
	}
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}

void Inventory::UnloadInventory()
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	if (!inventoryData->isDisplay) {
		return;
	}
	inventoryData->isDisplay = false;
	m_InventorySpace->GetSpriteComponent()->toDraw = inventoryData->isDisplay;
	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon+inventoryData->SlotCount; i++)
	{
		InventoryStoreData* data = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, data->SlotIndex);
		
		if (ECS::PixelSpriteComponent* sprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slot->SlotID)) {
			sprite->toDraw = inventoryData->isDisplay;
		}
		slot->SlotID = NULL;
		if (ECS::PixelSpriteComponent* pix = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(data->UITexID)) {
			pix->toDraw = inventoryData->isDisplay;
		}
	}
	for (int i = 0; i < inventoryData->SlotCount; i++)
	{
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, i);
		if (ECS::PixelSpriteComponent* sprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slot->SlotID)) {
			sprite->toDraw = inventoryData->isDisplay;
		}
		slot->SlotID = NULL;
	}
}

void OnDragChangeScaleSettings(ECS::EntityID id) {
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(id)->scaleSetting = ECS::e_Scale::Absolute;
}
bool Inventory::AddItem(Firelight::ECS::Entity* item)
{
	return AddItem(item->GetEntityID());
}

bool Inventory::AddItem(Firelight::ECS::EntityID item)
{
	bool isFail = true;
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	InventoryComponentInPut* inputCommands = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentInPut>(m_InventoryEntityID);

	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);
		InventoryComponentSpecialSlot* specialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(m_InventoryEntityID, slot->SpecialSlotIndex);

		if (slot->IsUsed == true && ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->itemID != ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->EntityIDs[0])->itemID) {
			//not free slot
			continue;
		}
		else {
			if (slotData->EntityIDs.size() > slotData->StackSize && slotData->StackSize != -1)
			{	//next slot		
				continue;
			}

			slotData->EntityIDs.push_back(item);

			if (!slot->IsUsed) {
				if (specialSlot) {
					//test tags
					bool isGood = true;
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->description;
					for (size_t j = 0; j < specialSlot->Tags.size(); j++)
					{
						
					}
				}
				//set icon data
				if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)) {
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->texture = Graphics::AssetManager::Instance().GetTexture(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->iconPath);
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = inventoryData->isDisplay;
					if (inventoryData->isDisplay) {
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID= slot->SlotID;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->hasParent = true;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->defaultDimensions=Maths::Vec3f(
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultScale.x,
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultScale.y,
							0);
						Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
					}
				}
				else
				{
					ECS::UIEntity* icon = new ECS::UIEntity();
					icon->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->iconPath);
					icon->GetSpriteComponent()->toDraw = inventoryData->isDisplay;
					icon->SetAnchorSettings(ECS::e_AnchorSettings::Center);
					icon->GetWidgetComponent()->hasParent = false;
					if (inventoryData->isDisplay) {
						icon->SetParent(slot->SlotID);
						icon->SetDefaultDimensions(Maths::Vec3f(
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultScale.x,
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultScale.y,
							0));
					}
					icon->AddComponent<ECS::UIDraggableComponent>();
					icon->GetComponent<ECS::UIDraggableComponent>()->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData));
					icon->GetComponent<ECS::UIDraggableComponent>()->onPickUpFunctions.push_back(std::bind(&OnDragChangeScaleSettings, icon->GetEntityID()));
					slotData->UITexID = icon->GetEntityID();
				}
				if (inventoryData->isDisplay) {
					Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
				}

				//Slot.second->CurrSlot = &Slot.first;
				slot->IsUsed = true;
				slotData->StackSize = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->stackSize - 1;
			}
			isFail = false;
			break;
		}

	}


	return isFail;
}

bool Inventory::AddItem(InventoryStoreData item, bool useSlotPlacement )
{
	bool isFail = true;
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	
	
	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);
		if (slot->IsUsed == true) {
			//not free slot
			if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item.UITexID)->parentID == slot->SlotID && useSlotPlacement) {
				isFail = AddItem(item, false);;
				break;
			}
			continue;
		}
		else if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item.UITexID)->parentID != slot->SlotID && useSlotPlacement)
		{
				continue;
		}
		else {
			slotData->UITexID = item.UITexID;
			slotData->StackSize = item.StackSize;
			slotData->EntityIDs = item.EntityIDs;

			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData));
			slot->IsUsed = true;

			
			if (inventoryData->isDisplay) {
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = inventoryData->isDisplay;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID = slot->SlotID;
			}
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->defaultDimensions = Maths::Vec3f(
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultScale.x,
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultScale.y,
				0);
			isFail = false;
			break;
		}

	 }
	
	return isFail;
}

bool Inventory::RemoveItem(Firelight::ECS::Entity* item)
{
	return RemoveItem(item->GetEntityID());
}

bool Inventory::RemoveItem(Firelight::ECS::EntityID item)
{
	bool isFail = true;
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);

		if (!slot->IsUsed) {
			continue;
		}
		else
		{
			std::vector<ECS::EntityID>::iterator iter = slotData->EntityIDs.begin();;
			while (iter != slotData->EntityIDs.end()) {
				if (*iter == item) {
					iter = slotData->EntityIDs.erase(iter);
					if (slotData->EntityIDs.size() == 0)
					{
						slot->IsUsed = false;
						slotData->StackSize = -1;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = false;
					}
					isFail = false;
					
				}
				else
				{
					++iter;
				}
			}
		}
	}
	return isFail;
}

void Inventory::Place(InventoryStoreData* slotData)
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	InventorySlots* currSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);


	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* placeSlotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* placeSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, placeSlotData->SlotIndex);
		if (placeSlot->SlotID == ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID) {
			if (placeSlot->IsUsed && !inventoryData->isSwap) {
				return;
			}

			InventoryStoreData tempSlotData = *placeSlotData;

			placeSlotData->EntityIDs = slotData->EntityIDs;
			placeSlotData->StackSize = slotData->StackSize;
			placeSlotData->UITexID = slotData->UITexID;

			slotData->EntityIDs= tempSlotData.EntityIDs;
			slotData->StackSize = tempSlotData.StackSize;
			slotData->UITexID = tempSlotData.UITexID;



			placeSlot->IsUsed = true;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(placeSlotData->UITexID)->parentID = placeSlot->SlotID;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(placeSlotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(placeSlotData->UITexID)->onDropUpFunctions.clear();
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(placeSlotData->UITexID)->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, placeSlotData));

			if (slotData->StackSize > -1) {
				
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID = currSlot->SlotID;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.clear();
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData));
				currSlot->IsUsed = true;
			}
			else
			{
				currSlot->IsUsed = false;
			}

			return;

		}
	}

	////not in inventory
	//remove from inventory 
	currSlot->IsUsed = false;
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = false;

	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.clear();
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Absolute;

	InventoryStoreData dataCopy = *slotData;
	slotData->EntityIDs.clear();
	slotData->StackSize = -1;
	slotData->UITexID = 0;

	//to be used some were else
	m_OutOfInventoryData.push_back(dataCopy);

	Events::EventDispatcher::InvokeFunctions<Events::Inv::UPDATEINV>();

}

ECS::EntityID Inventory::GetSpecialSlot(std::string Name)
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);
		InventoryComponentSpecialSlot* specialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(m_InventoryEntityID, slot->SpecialSlotIndex);
		if (specialSlot) {
			if (specialSlot->SlotName == Name) {
				return slotData->EntityIDs[0];
			}
		}

	}
	return -1;
}

bool Inventory::FindItem(Firelight::ECS::Entity* item)
{
	return FindItem(item->GetEntityID());
}

bool Inventory::FindItem(Firelight::ECS::EntityID item)
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);
		if (!slot->IsUsed) {
			continue;
		}
		else
		{
			for (auto& items : slotData->EntityIDs)
			{
				if (items == item) {
					return true;
				}
			}
		}
	}
	return false;
}

bool Inventory::RemoveItemType(int howMany, int type)
{
	bool isFail = true;
	std::vector<InventoryStoreData*> dataToRemove;
	int countToRemove = howMany;
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);
		if (!slot->IsUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->EntityIDs[0])->itemID != type) {
			continue;
		}
		countToRemove -= slotData->EntityIDs.size();
		dataToRemove.push_back(slotData);

		if (countToRemove <= 0) {
			for (size_t i = 0; i < dataToRemove.size(); i++)
			{
				for (size_t j = 0; j < howMany; /*j++,*/ howMany--)
				{
					dataToRemove[i]->EntityIDs.erase(dataToRemove[i]->EntityIDs.begin() + (dataToRemove[i]->EntityIDs.size() - 1));

					if (dataToRemove[i]->EntityIDs.size() == 0)
					{
						InventorySlots* slotForRemove = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, dataToRemove[i]->SlotIndex);
						slotForRemove->IsUsed = false;
						dataToRemove[i]->StackSize = -1;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(dataToRemove[i]->UITexID)->toDraw = false;
						break;
					}
				}
			}
			isFail = false;
			return isFail;
		}
	}
	return isFail;
}

int Inventory::GetItemTypeTotal(int type)
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	int total = 0;
	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);
		if (!slot->IsUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->EntityIDs[0])->itemID != type) {
			continue;
		}
		total += slotData->EntityIDs.size();
	}
	return total;
}

std::vector<ECS::EntityID> Inventory::GetItemType(int howMany, int type)
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_InventoryEntityID, m_GroupInventoryID);
	std::vector<InventoryStoreData*> dataToGet;
	std::vector<ECS::EntityID> returnIDData;
	int countToAdd = howMany;
	for (int i = inventoryData->SlotStartPositon; i < inventoryData->SlotStartPositon + inventoryData->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_InventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_InventoryEntityID, slotData->SlotIndex);
		if (!slot->IsUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->EntityIDs[0])->itemID != type) {
			continue;
		}
		countToAdd -= slotData->EntityIDs.size();
		dataToGet.push_back(slotData);

		if (countToAdd <= 0) {
			for (size_t i = 0; i < dataToGet.size(); i++)
			{
				int index = 0;
				for (size_t j = 0; j < howMany; howMany--)
				{
					returnIDData.push_back(dataToGet[i]->EntityIDs[index]);
					if (howMany == 0)
					{
						return returnIDData;
					}
					index++;
				}
			}
		}
	}
	return returnIDData;
}
