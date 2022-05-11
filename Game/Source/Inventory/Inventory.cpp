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
	ECS::EntityComponentSystem::Instance()->RemoveEntity(InventorySpace->GetEntityID());
}

void Inventory::SetEntityData(ECS::EntityID ID, int InvListPos = 0) {
	IveID = ID;
	invetoryPos = InvListPos;
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	InventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(Iventory->BackgroundTex);
}

void Inventory::CreatInventoryNoPannel(Maths::Vec2f size, float slotCount, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f OffSet)
{
	InventorySpace = new ECS::UIPanel();
	InventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	InventorySpace->GetSpriteComponent()->toDraw = false;
	InventorySpace->SetAnchorSettings(Anchor);
	InventorySpace->SetParent(parent);
	InventorySpace->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	InventorySpace->SetOffset(OffSet);
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}
void Inventory::CreatInventoryNoPannel(Maths::Vec2f size, Maths::Vec2f rows, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f OffSet)
{
	InventorySpace = new ECS::UIPanel();
	InventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	InventorySpace->GetSpriteComponent()->toDraw = false;
	InventorySpace->SetAnchorSettings(Anchor);
	InventorySpace->SetParent(parent);
	InventorySpace->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	InventorySpace->SetOffset(OffSet);
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}

//draw on screen
void Inventory::LoadInventory(std::vector<ECS::UIPanel*> *PannleToUse, bool ToFit)
{
	InventoryComponent* Iventory= ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);

	if (Iventory->isDisplay) {
		return;
	}
	Iventory->isDisplay = true;
	InventorySpace->GetSpriteComponent()->toDraw = Iventory->isDisplay;

	if (ToFit) {
		//number of slots per row
		float ColoumCount = InventorySpace->GetWidgetComponent()->defaultDimensions.x / Iventory->SlotCount;
		if (ColoumCount == Iventory->SlotCount) {
			ColoumCount = 1.0f;
		}
		//number of rows
		float RowCount = Iventory->SlotCount / ColoumCount;
	}
	float sizeY = InventorySpace->GetWidgetComponent()->defaultDimensions.y / Iventory->RowCount;
	float sizeX = InventorySpace->GetWidgetComponent()->defaultDimensions.x / Iventory->ColoumCount;

	

	float currX = 0;
	float currY = 0;
	int currentPos = Iventory->SlotStartPositon;
	int nextFreePannle = -1;
	for (size_t i = 0; i < PannleToUse->size(); i++)
	{
		if (!PannleToUse->at(i)->GetSpriteComponent()->toDraw) {
			nextFreePannle = i;
			break;
		}

	}

	for (size_t i = 0; i < Iventory->RowCount; i++)
	{
		for (size_t i = 0; i < Iventory->ColoumCount; i++)
		{
			ECS::UIPanel* Slot;
			//panel to use
			if (PannleToUse->size() > nextFreePannle && nextFreePannle != -1){
				//use exsting pannle
				Slot = PannleToUse->at(nextFreePannle);
				Slot->GetSpriteComponent()->texture=Graphics::AssetManager::Instance().GetTexture(Iventory->SlotTex);
				Slot->GetSpriteComponent()->toDraw = Iventory->isDisplay;
				Slot->SetParent(InventorySpace->GetEntityID());
				Slot->SetDefaultDimensions(Maths::Vec3f(sizeX / InventorySpace->GetWidgetComponent()->currentScale.x, sizeY / InventorySpace->GetWidgetComponent()->currentScale.y, 0));
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
				Slot->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(Iventory->SlotTex);
				Slot->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
				Slot->SetParent(InventorySpace->GetEntityID());
				Slot->SetDefaultDimensions(Maths::Vec3f(sizeX / InventorySpace->GetWidgetComponent()->currentScale.x, sizeY / InventorySpace->GetWidgetComponent()->currentScale.y, 0));
				Slot->SetOffset(Maths::Vec2f(currX, currY));
				PannleToUse->push_back(Slot);
			}
			currX += sizeX;
			
			ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, currentPos)->SlotID = Slot->GetEntityID();
			
			currentPos++;
		}
		currY += sizeY;
		currX = 0;
	}
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		
		InventoryStoreData* data = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, data->SlotIndex);
		InventoryComponentSpecialSlot* SpecialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(IveID, slot->SpecialSlotIndex);

		
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
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	if (!Iventory->isDisplay) {
		return;
	}
	Iventory->isDisplay = false;
	InventorySpace->GetSpriteComponent()->toDraw = Iventory->isDisplay;
	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* data = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, data->SlotIndex);
		
		if (ECS::PixelSpriteComponent* sprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slot->SlotID)) {
			sprite->toDraw = Iventory->isDisplay;
		}
		slot->SlotID = NULL;
		if (ECS::PixelSpriteComponent* pix = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(data->UITexID)) {
			pix->toDraw = Iventory->isDisplay;
		}
	}
	for (int i = 0; i < Iventory->SlotCount; i++)
	{
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, i);
		if (ECS::PixelSpriteComponent* sprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slot->SlotID)) {
			sprite->toDraw = Iventory->isDisplay;
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
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	InventoryComponentInPut* InputCommands = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentInPut>(IveID);

	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);
		InventoryComponentSpecialSlot* SpecialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(IveID, slot->SpecialSlotIndex);

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
				if (SpecialSlot) {
					//test tags
					bool isGood = true;
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->description;
					for (size_t j = 0; j < SpecialSlot->Tags.size(); j++)
					{
						
					}
				}
				if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)) {
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->texture = Graphics::AssetManager::Instance().GetTexture(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->iconPath);
					ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = Iventory->isDisplay;
					if (Iventory->isDisplay) {
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
					ECS::UIEntity* Immage = new ECS::UIEntity();
					Immage->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->iconPath);
					Immage->GetSpriteComponent()->toDraw = Iventory->isDisplay;
					Immage->SetAnchorSettings(ECS::e_AnchorSettings::Center);
					Immage->GetWidgetComponent()->hasParent = false;
					if (Iventory->isDisplay) {
						Immage->SetParent(slot->SlotID);
						Immage->SetDefaultDimensions(Maths::Vec3f(
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultScale.x,
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->SlotID)->defaultScale.y,
							0));
					}
					Immage->AddComponent<ECS::UIDraggableComponent>();
					Immage->GetComponent<ECS::UIDraggableComponent>()->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData));
					Immage->GetComponent<ECS::UIDraggableComponent>()->onPickUpFunctions.push_back(std::bind(&OnDragChangeScaleSettings, Immage->GetEntityID()));
					slotData->UITexID = Immage->GetEntityID();
				}
				if (Iventory->isDisplay) {
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
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	
	
	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);
		if (slot->IsUsed == true) {
			//not free slot
			if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item.UITexID)->parentID == slot->SlotID && useSlotPlacement) {
				AddItem(item, false);
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

			
			if (Iventory->isDisplay) {
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = Iventory->isDisplay;
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
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);

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
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);


	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData2 = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot2 = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData2->SlotIndex);
		if (slot2->SlotID == ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID) {
			if (slot2->IsUsed && !Iventory->isSwap) {
				return;
			}

			InventoryStoreData temp = *slotData2;

			slotData2->EntityIDs = slotData->EntityIDs;
			slotData2->StackSize = slotData->StackSize;
			slotData2->UITexID = slotData->UITexID;

			slotData->EntityIDs= temp.EntityIDs;
			slotData->StackSize = temp.StackSize;
			slotData->UITexID = temp.UITexID;



			slot2->IsUsed = true;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData2->UITexID)->parentID = slot2->SlotID;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData2->UITexID)->scaleSetting = ECS::e_Scale::Relative;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData2->UITexID)->onDropUpFunctions.clear();
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData2->UITexID)->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData2));

			if (slotData->StackSize > -1) {
				
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID = slot->SlotID;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.clear();
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData));
				slot->IsUsed = true;
			}
			else
			{
				slot->IsUsed = false;
			}

			return;

		}
	}

	////not in inventory
	//remove from inventory 
	slot->IsUsed = false;
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = false;

	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.clear();
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Absolute;

	InventoryStoreData DataCoppy = *slotData;
	slotData->EntityIDs.clear();
	slotData->StackSize = -1;
	slotData->UITexID = 0;

	//to be used some were else
	NullSlotData.push_back(DataCoppy);

	Events::EventDispatcher::InvokeFunctions<Events::Inv::UPDATEINV>();

}

ECS::EntityID Inventory::GetSpecialSlot(std::string Name)
{
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);
		InventoryComponentSpecialSlot* SpecialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(IveID, slot->SpecialSlotIndex);
		if (SpecialSlot) {
			if (SpecialSlot->SlotName == Name) {
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
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);
		if (!slot->IsUsed) {
			continue;
		}
		else
		{
			for (auto& Items : slotData->EntityIDs)
			{
				if (Items == item) {
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
	std::vector<InventoryStoreData*> DataToRemove;
	int countToRemove = howMany;
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);
		if (!slot->IsUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->EntityIDs[0])->itemID != type) {
			continue;
		}
		countToRemove -= slotData->EntityIDs.size();
		DataToRemove.push_back(slotData);

		if (countToRemove <= 0) {
			for (size_t i = 0; i < DataToRemove.size(); i++)
			{
				for (size_t j = 0; j < howMany; /*j++,*/ howMany--)
				{
					DataToRemove[i]->EntityIDs.erase(DataToRemove[i]->EntityIDs.begin() + (DataToRemove[i]->EntityIDs.size() - 1));

					if (DataToRemove[i]->EntityIDs.size() == 0)
					{
						InventorySlots* slotForRemove = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, DataToRemove[i]->SlotIndex);
						slotForRemove->IsUsed = false;
						DataToRemove[i]->StackSize = -1;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(DataToRemove[i]->UITexID)->toDraw = false;
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
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	int total = 0;
	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);
		if (!slot->IsUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->EntityIDs[0])->itemID != type) {
			continue;
		}
		total += slotData->EntityIDs.size();
	}
	return total;
}

std::vector<ECS::EntityID> Inventory::GetItemType(int howMany, int type)
{
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos);
	std::vector<InventoryStoreData*> DataToAdd;
	std::vector<ECS::EntityID> ids;
	int countToAdd = howMany;
	for (int i = Iventory->SlotStartPositon; i < Iventory->SlotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(IveID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(IveID, slotData->SlotIndex);
		if (!slot->IsUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->EntityIDs[0])->itemID != type) {
			continue;
		}
		countToAdd -= slotData->EntityIDs.size();
		DataToAdd.push_back(slotData);

		if (countToAdd <= 0) {
			for (size_t i = 0; i < DataToAdd.size(); i++)
			{
				int index = 0;
				for (size_t j = 0; j < howMany; howMany--)
				{
					ids.push_back(DataToAdd[i]->EntityIDs[index]);
					if (howMany == 0)
					{
						return ids;
					}
					index++;
				}
			}
		}
	}
	return ids;
}
