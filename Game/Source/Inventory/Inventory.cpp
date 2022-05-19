#include "Inventory.h"
#include "Source/Graphics/AssetManager.h"
#include"Source/ECS/Components/ItemComponents.h"

Inventory::Inventory()
{
}

Inventory::Inventory(std::string name)
{
}

Inventory::~Inventory()
{
	ECS::EntityComponentSystem::Instance()->RemoveEntity(m_inventorySpace->GetEntityID());
}

void Inventory::SetEntityData(ECS::EntityID ID, int InvListPos) 
{
	m_inventoryEntityID = ID;
	m_groupInventoryID = InvListPos;
	InventoryComponent* Iventory = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	m_inventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(Iventory->backgroundTexture);
}

void Inventory::CreateInventoryNoPanel(Maths::Vec2f size, float slotCount, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f offset)
{
	m_inventorySpace = new ECS::UIPanel("Inventory Space");
	m_inventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	m_inventorySpace->GetSpriteComponent()->toDraw = false;
	m_inventorySpace->SetAnchorSettings(Anchor);
	m_inventorySpace->SetParent(parent);
	m_inventorySpace->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	m_inventorySpace->SetOffset(offset);
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}
void Inventory::CreateInventoryNoPanel(Maths::Vec2f size, Maths::Vec2f rows, ECS::EntityID parent, ECS::e_AnchorSettings anchor, Maths::Vec2f offset)
{
	m_inventorySpace = new ECS::UIPanel("Inventory Space");
	m_inventorySpace->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture("Sprites/PanelTest.png");
	m_inventorySpace->GetSpriteComponent()->toDraw = false;
	m_inventorySpace->GetWidgetComponent()->isActive = true;
	m_inventorySpace->SetAnchorSettings(anchor);
	m_inventorySpace->SetParent(parent);
	m_inventorySpace->SetDefaultDimensions(Maths::Vec3f(size.x, size.y, 0));
	m_inventorySpace->SetOffset(offset);
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}

//draw on screen
void Inventory::LoadInventory(std::vector<ECS::UIPanel*>* panelToUse, bool toFit)
{
	InventoryComponent* inventoryData= ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);

	if (inventoryData->isDisplay) 
	{
		return;
	}
	inventoryData->isDisplay = true;
	m_inventorySpace->GetSpriteComponent()->toDraw = inventoryData->isDisplay;
	m_inventorySpace->GetWidgetComponent()->isActive = inventoryData->isDisplay;
	if (toFit) 
	{
		//number of slots per row
		float columnCount = m_inventorySpace->GetWidgetComponent()->defaultDimensions.x / inventoryData->slotCount;
		if (columnCount == inventoryData->slotCount) 
		{
			columnCount = 1.0f;
		}
		//number of rows
		float rowCount = inventoryData->slotCount / columnCount;
	}
	float sizeY = m_inventorySpace->GetWidgetComponent()->defaultDimensions.y / inventoryData->rowCount;
	float sizeX = m_inventorySpace->GetWidgetComponent()->defaultDimensions.x / inventoryData->columnCount;

	

	float currX = 0;
	float currY = 0;
	int currentPos = inventoryData->slotStartPositon;
	int nextFreePannle = -1;
	for (size_t i = 0; i < panelToUse->size(); i++)
	{
		if (!panelToUse->at(i)->GetSpriteComponent()->toDraw) 
		{
			nextFreePannle = i;
			break;
		}

	}

	for (size_t i = 0; i < inventoryData->rowCount; i++)
	{
		for (size_t j = 0; j < inventoryData->columnCount; j++)
		{
			ECS::UIPanel* slot;
			//panel to use
			if (panelToUse->size() - 1 >= nextFreePannle && nextFreePannle != -1)
			{
				//use exsting pannle
				slot = panelToUse->at(nextFreePannle);
				slot->GetSpriteComponent()->texture=Graphics::AssetManager::Instance().GetTexture(inventoryData->slotTexture);
				slot->GetSpriteComponent()->toDraw = inventoryData->isDisplay;
				slot->GetWidgetComponent()->isActive = true;
				slot->SetParent(m_inventorySpace->GetEntityID());
				slot->SetDefaultDimensions(Maths::Vec3f(sizeX / m_inventorySpace->GetWidgetComponent()->currentScale.x, sizeY / m_inventorySpace->GetWidgetComponent()->currentScale.y, 0));
				slot->SetOffset(Maths::Vec2f(currX, currY));
				slot->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
				
				//find next non drawn pannle
				bool isFound = false;
				for (size_t k = 0; k < panelToUse->size(); k++)
				{
					if (!panelToUse->at(k)->GetSpriteComponent()->toDraw) 
					{
						nextFreePannle = k;
						isFound = true;
						break;
					}

				}
				if (!isFound) {
					nextFreePannle++;
				}
			}
			else
			{
				//new pannle
				slot = new ECS::UIPanel("Inventory Slot");
				slot->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(inventoryData->slotTexture);
				slot->SetAnchorSettings(ECS::e_AnchorSettings::TopLeft);
				slot->SetParent(m_inventorySpace->GetEntityID());
				slot->SetDefaultDimensions(Maths::Vec3f(sizeX / m_inventorySpace->GetWidgetComponent()->currentScale.x, sizeY / m_inventorySpace->GetWidgetComponent()->currentScale.y, 0));
				slot->SetOffset(Maths::Vec2f(currX, currY));
				slot->GetWidgetComponent()->isActive = inventoryData->isDisplay;
				TextComponent* text = slot->AddComponent<ECS::TextComponent>();
				text->hidden = true;
				text->text.SetTextHeight(20);
				text->layer = 100000;
				panelToUse->push_back(slot);
				nextFreePannle= panelToUse->size();
			}
			currX += sizeX;
			
			ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, currentPos)->slotID = slot->GetEntityID();
			
			currentPos++;
		}
		currY += sizeY;
		currX = 0;
	}
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();

	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		
		InventoryStoreData* data = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, data->slotIndex);
		InventoryComponentSpecialSlot* specialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(m_inventoryEntityID, slot->specialSlotIndex);

		
		if (specialSlot) {
			ECS::EntityComponentSystem::Instance()->GetComponent< Firelight::ECS::UIBaseWidgetComponent >(slot->slotID)->offSet = specialSlot->offset;
			ECS::EntityComponentSystem::Instance()->GetComponent< Firelight::ECS::UIBaseWidgetComponent >(slot->slotID)->anchorSettings = specialSlot->anchorSettings;
			ECS::EntityComponentSystem::Instance()->GetComponent< Firelight::ECS::UIBaseWidgetComponent >(slot->slotID)->defaultDimensions = (Maths::Vec3f(specialSlot->size.x / m_inventorySpace->GetWidgetComponent()->currentScale.x, specialSlot->size.y / m_inventorySpace->GetWidgetComponent()->currentScale.y, 0));
			
		}

		if (data->stackSize > -1) {
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(data->UITexID)->hasParent = true;
			ECS::PixelSpriteComponent* iconSprite =ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(data->UITexID);
			iconSprite->toDraw = true;
			
			ECS::UIBaseWidgetComponent* iconWidget = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(data->UITexID);
			ECS::UIBaseWidgetComponent* slotWidget = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID);
			iconWidget->parentID = slot->slotID;
			iconWidget->isActive = true;
			iconWidget->defaultDimensions = Maths::Vec3f(
				slotWidget->defaultDimensions.x / slotWidget->defaultScale.x,
				slotWidget->defaultDimensions.y / slotWidget->defaultScale.y,
				0);
			iconWidget->hasParent = true;
			TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(data->UITexID);
			text->hidden = false;
			text->text.SetString(std::to_string(data->entityIDs.size()));
			//text->layer = iconSprite->layer + 1;

		}
	}
	Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
}

void Inventory::UnloadInventory()
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	if (!inventoryData->isDisplay) {
		return;
	}
	inventoryData->isDisplay = false;
	m_inventorySpace->GetSpriteComponent()->toDraw = inventoryData->isDisplay;
	m_inventorySpace->GetWidgetComponent()->isActive = inventoryData->isDisplay;
	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon+inventoryData->slotCount; i++)
	{
		InventoryStoreData* data = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, data->slotIndex);
		
		if (ECS::PixelSpriteComponent* sprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slot->slotID)) 
		{
			sprite->toDraw = inventoryData->isDisplay;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->isActive = inventoryData->isDisplay;
		}
		slot->slotID = NULL;
		if (ECS::PixelSpriteComponent* pix = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(data->UITexID))
		{
			pix->toDraw = inventoryData->isDisplay;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(data->UITexID)->isActive = inventoryData->isDisplay;
			TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(data->UITexID);
			text->hidden = true;
		}
	}
	for (int i = 0; i < inventoryData->slotCount; i++)
	{
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, i);
		if (ECS::PixelSpriteComponent* sprite = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slot->slotID)) 
		{
			sprite->toDraw = inventoryData->isDisplay;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->isActive = inventoryData->isDisplay;
		}
		slot->slotID = NULL;
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
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	InventoryComponentInPut* inputCommands = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponentInPut>(m_inventoryEntityID);

	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);
		InventoryComponentSpecialSlot* specialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(m_inventoryEntityID, slot->specialSlotIndex);

		if (slot->isUsed == true && ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->itemID != ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->entityIDs[0])->itemID) 
		{
			//not free slot
			continue;
		}
		else {
			if (slotData->entityIDs.size() > slotData->stackSize && slotData->stackSize != -1)
			{	//next slot		
				continue;
			}

			slotData->entityIDs.push_back(item);

			if (!slot->isUsed) {
				bool isGood = true;
				if (specialSlot) {
					//test tags

					std::vector<std::string> tagData = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->tags;
					for (size_t j = 0; j < specialSlot->tags.size(); j++)
					{
						if (std::find(tagData.begin(), tagData.end(), specialSlot->tags[j]) == tagData.end()) {
							isGood = false;
						}
					}
				}
				if (isGood) {
					//set icon data
					if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)) {
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->texture = Graphics::AssetManager::Instance().GetTexture(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->iconPath);
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = inventoryData->isDisplay;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->isActive = inventoryData->isDisplay;
						if (inventoryData->isDisplay) {
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID = slot->slotID;
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->hasParent = true;
							ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->defaultDimensions = Maths::Vec3f(
								ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultScale.x,
								ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultScale.y,
								0);
							Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
							TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(slotData->UITexID);
							text->hidden = false;
							//text->text.SetString(std::to_string(slotData->entityIDs.size()));
							
						}
					}
					else
					{
						ECS::UIEntity* icon = new ECS::UIEntity("Inventory Icon");
						icon->GetSpriteComponent()->texture = Graphics::AssetManager::Instance().GetTexture(ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->iconPath);
						icon->GetSpriteComponent()->toDraw = inventoryData->isDisplay;
						icon->SetAnchorSettings(ECS::e_AnchorSettings::Center);
						icon->GetWidgetComponent()->hasParent = false;
						icon->GetWidgetComponent()->isActive = inventoryData->isDisplay;
						TextComponent* text = icon->AddComponent<ECS::TextComponent>();
						text->hidden = true;
						text->text.SetTextHeight(30);
						text->layer = 100000;
						text->text.SetTextAnchor(Firelight::Graphics::TextAnchor::e_BotRight);

						if (inventoryData->isDisplay) {
							icon->SetParent(slot->slotID);
							icon->SetDefaultDimensions(Maths::Vec3f(
								ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultScale.x,
								ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultScale.y,
								0));
							
							text->hidden = false;
							
						}
						icon->AddComponent<ECS::UIDraggableComponent>();
						icon->GetComponent<ECS::UIDraggableComponent>()->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData));
						icon->GetComponent<ECS::UIDraggableComponent>()->onPickUpFunctions.push_back(std::bind(&OnDragChangeScaleSettings, icon->GetEntityID()));
						slotData->UITexID = icon->GetEntityID();
					}
					if (inventoryData->isDisplay) {
						Events::EventDispatcher::InvokeFunctions<Events::UI::UpdateUIEvent>();
					}

					//slot.second->CurrSlot = &slot.first;
					
				}
				
			}		
					slot->isUsed = true;
					slotData->stackSize = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item)->stackSize - 1;
					if (inventoryData->isDisplay) {
						TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(slotData->UITexID);
						text->text.SetString(std::to_string(slotData->entityIDs.size()));
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
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	
	
	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);
		InventoryComponentSpecialSlot* specialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(m_inventoryEntityID, slot->specialSlotIndex);
		if (slot->isUsed == true) 
		{
			//not free slot
			if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item.UITexID)->parentID == slot->slotID && useSlotPlacement) 
			{
				isFail = AddItem(item, false);
				break;
			}
			continue;
		}
		else if (ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(item.UITexID)->parentID != slot->slotID && useSlotPlacement)
		{
				continue;
		}
		else 
		{

			bool isGood = true;
			if (specialSlot) {
				//test tags

				std::vector<std::string> tagData = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(item.entityIDs[0])->tags;
				for (size_t j = 0; j < specialSlot->tags.size(); j++)
				{
					if (std::find(tagData.begin(), tagData.end(), specialSlot->tags[j]) == tagData.end()) {
						isGood = false;
						
					} 
				}
			}
			if (!isGood) {
				if (useSlotPlacement) {
					isFail = AddItem(item, false);
					break;
				}


				continue;
			}



			slotData->UITexID = item.UITexID;
			slotData->stackSize = item.stackSize;
			slotData->entityIDs = item.entityIDs;

			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData));
			slot->isUsed = true;

			
			if (inventoryData->isDisplay) {
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = inventoryData->isDisplay;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->isActive = inventoryData->isDisplay;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID = slot->slotID;
				TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(slotData->UITexID);
				text->hidden = false;
				text->text.SetString(std::to_string(slotData->entityIDs.size()));
			}
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->defaultDimensions = Maths::Vec3f(
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultDimensions.x / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultScale.x,
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultDimensions.y / ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slot->slotID)->defaultScale.y,
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
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);

		if (!slot->isUsed) {
			continue;
		}
		else
		{
			std::vector<ECS::EntityID>::iterator iter = slotData->entityIDs.begin();
			while (iter != slotData->entityIDs.end()) 
			{
				if (*iter == item)
				{
					iter = slotData->entityIDs.erase(iter);
					if (slotData->entityIDs.size() == 0)
					{
						slot->isUsed = false;
						slotData->stackSize = -1;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = false;
						TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(slotData->UITexID);
						text->hidden = true;
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
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	InventorySlots* currSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);


	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* placeSlotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* placeSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, placeSlotData->slotIndex);
		InventoryComponentSpecialSlot* specialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(m_inventoryEntityID, placeSlot->specialSlotIndex);
	
		if (placeSlot->slotID == ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID)
		{

			if (placeSlot->isUsed && !inventoryData->isSwap) 
			{
				return;
			}
			bool isGood = true;

			if (specialSlot) {
				//test tags

				std::vector<std::string> tagData = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->entityIDs[0])->tags;
				for (size_t j = 0; j < specialSlot->tags.size(); j++)
				{
					if (std::find(tagData.begin(), tagData.end(), specialSlot->tags[j]) == tagData.end()) {
						isGood = false;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID = currSlot->slotID;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
						Events::EventDispatcher::InvokeFunctions<Events::Inventory::UpdateInventory>();
						return;
					}
				}
			}

			

			InventoryStoreData tempSlotData = *placeSlotData;

			placeSlotData->entityIDs = slotData->entityIDs;
			placeSlotData->stackSize = slotData->stackSize;
			placeSlotData->UITexID = slotData->UITexID;

			slotData->entityIDs= tempSlotData.entityIDs;
			slotData->stackSize = tempSlotData.stackSize;
			slotData->UITexID = tempSlotData.UITexID;



			placeSlot->isUsed = true;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(placeSlotData->UITexID)->parentID = placeSlot->slotID;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(placeSlotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(placeSlotData->UITexID)->onDropUpFunctions.clear();
			ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(placeSlotData->UITexID)->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, placeSlotData));
			TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(placeSlotData->UITexID);
			text->hidden = false;
			text->text.SetString(std::to_string(placeSlotData->entityIDs.size()));
			if (slotData->stackSize > -1) 
			{
				
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->parentID = currSlot->slotID;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Relative;
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.clear();
				ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.push_back(std::bind(&Inventory::Place, this, slotData));
				TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(slotData->UITexID);
				text->hidden = false;
				text->text.SetString(std::to_string(slotData->entityIDs.size()));
				currSlot->isUsed = true;
			}
			else
			{
				currSlot->isUsed = false;
			}

			return;

		}
	}

	////not in inventory
	//remove from inventory 
	currSlot->isUsed = false;
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(slotData->UITexID)->toDraw = false;
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->isActive = false;
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIDraggableComponent>(slotData->UITexID)->onDropUpFunctions.clear();
	ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(slotData->UITexID)->scaleSetting = ECS::e_Scale::Absolute;
	TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(slotData->UITexID);
	text->hidden = true;
	
	InventoryStoreData dataCopy = *slotData;
	slotData->entityIDs.clear();
	slotData->stackSize = -1;
	slotData->UITexID = 0;

	//to be used some were else
	m_outOfInventoryData.push_back(dataCopy);

	Events::EventDispatcher::InvokeFunctions<Events::Inventory::UpdateInventory>();

}

ECS::EntityID Inventory::GetSpecialSlot(std::string name)
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);
		InventoryComponentSpecialSlot* specialSlot = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryComponentSpecialSlot >(m_inventoryEntityID, slot->specialSlotIndex);
		if (specialSlot) 
		{
			if (specialSlot->slotName == name) 
			{
				return slotData->entityIDs[0];
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
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);
		if (!slot->isUsed)
		{
			continue;
		}
		else
		{
			for (auto& items : slotData->entityIDs)
			{
				if (items == item) 
				{
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
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);
		if (!slot->isUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->entityIDs[0])->itemID != type) 
		{
			continue;
		}
		countToRemove -= slotData->entityIDs.size();
		dataToRemove.push_back(slotData);

		if (countToRemove <= 0) {
			for (size_t i = 0; i < dataToRemove.size(); i++)
			{
				for (size_t j = 0; j < howMany; /*j++,*/ howMany--)
				{
					dataToRemove[i]->entityIDs.erase(dataToRemove[i]->entityIDs.begin() + (dataToRemove[i]->entityIDs.size() - 1));

					if (dataToRemove[i]->entityIDs.size() == 0)
					{
						InventorySlots* slotForRemove = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, dataToRemove[i]->slotIndex);
						slotForRemove->isUsed = false;
						dataToRemove[i]->stackSize = -1;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::PixelSpriteComponent>(dataToRemove[i]->UITexID)->toDraw = false;
						ECS::EntityComponentSystem::Instance()->GetComponent<ECS::UIBaseWidgetComponent>(dataToRemove[i]->UITexID)->isActive = false;
						TextComponent* text = ECS::EntityComponentSystem::Instance()->GetComponent<ECS::TextComponent>(dataToRemove[i]->UITexID);
						text->hidden = true;
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
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	int total = 0;
	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);
		if (!slot->isUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->entityIDs[0])->itemID != type)
		{
			continue;
		}
		total += slotData->entityIDs.size();
	}
	return total;
}

std::vector<ECS::EntityID> Inventory::GetItemType(int howMany, int type)
{
	InventoryComponent* inventoryData = ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID);
	std::vector<InventoryStoreData*> dataToGet;
	std::vector<ECS::EntityID> returnIDData;
	int countToAdd = howMany;
	for (int i = inventoryData->slotStartPositon; i < inventoryData->slotStartPositon + inventoryData->slotCount; i++)
	{
		InventoryStoreData* slotData = ECS::EntityComponentSystem::Instance()->GetComponent< InventoryStoreData >(m_inventoryEntityID, i);
		InventorySlots* slot = ECS::EntityComponentSystem::Instance()->GetComponent< InventorySlots >(m_inventoryEntityID, slotData->slotIndex);
		if (!slot->isUsed || ECS::EntityComponentSystem::Instance()->GetComponent<ECS::ItemComponent>(slotData->entityIDs[0])->itemID != type)
		{
			continue;
		}
		countToAdd -= slotData->entityIDs.size();
		dataToGet.push_back(slotData);

		if (countToAdd <= 0)
		{
			for (size_t i = 0; i < dataToGet.size(); i++)
			{
				int index = 0;
				for (size_t j = 0; j < howMany; howMany--)
				{
					returnIDData.push_back(dataToGet[i]->entityIDs[index]);
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
