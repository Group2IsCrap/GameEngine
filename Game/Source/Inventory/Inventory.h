#pragma once

#include <Source/ECS/Components/UIComponents.h>
#include <Source/ECS/EntityWrappers/UIEntity.h>
#include <Source/ECS/EntityWrappers/UIPanel.h>
#include <Source/ECS/Systems/UISystem.h>
#include<Source/Maths/Vec2.h>
#include <string>
#include <vector>

#include"InventoryComponents.h"
#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"

using namespace Firelight;

class Inventory
{
public:
	Inventory();
	Inventory(std::string name);
	~Inventory();

	void SetEntityData(ECS::EntityID ID, int InvListPos=0);
	
	ECS::EntityID GetEntityData() { return m_inventoryEntityID; }
	int GetInventoryNumber() { return m_groupInventoryID; }
	void CreateInventory(Maths::Vec2f size, float slotCount, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f offset);
	void CreateInventory(Maths::Vec2f size, Maths::Vec2f rows, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f offset);

	//display controlls
	void LoadInventory(std::vector<ECS::UIPanel*> *panelToUse, bool toFit);
	void UnloadInventory();

	//item controlls
	bool AddItem(Firelight::ECS::Entity* item);
	bool AddItem(Firelight::ECS::EntityID item);
	bool AddItem(InventoryStoreData item, bool useSlotPlacement);

	bool RemoveItem(Firelight::ECS::Entity* item);
	bool RemoveItem(Firelight::ECS::EntityID item);

	bool FindItem(Firelight::ECS::Entity* item);
	bool FindItem(Firelight::ECS::EntityID item);

	bool RemoveItemType(int howMany, int type);
	int GetItemTypeTotal(int type);
	std::vector<ECS::EntityID> GetItemType(int howMany, int type);

	void Place(InventoryStoreData* slotData);

	ECS::EntityID GetSpecialSlot(std::string name);

	std::vector <InventoryStoreData>* GetNullSlotData() { return &m_outOfInventoryData; }
	ECS::UIPanel* GetInventorySpace() { return m_inventorySpace; }
	std::string GetName() { return ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID)->name; }
	bool GetIsDisplay() { return ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(m_inventoryEntityID, m_groupInventoryID)->isDisplay; }
private:



private:
	//panel created by this 
	ECS::UIPanel* m_inventorySpace;

	//inventory entity
	ECS::EntityID m_inventoryEntityID;
	int m_groupInventoryID = 0;

	//slots to remove
	std::vector <InventoryStoreData> m_outOfInventoryData;
};

