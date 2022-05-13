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
	Inventory(std::string Name);
	~Inventory();

	void SetEntityData(ECS::EntityID ID, int InvListPos=0);
	
	
	ECS::EntityID GetEntityData() { return IveID; }
	int GetInvetorNumber() { return invetoryPos; }
	void CreatInventoryNoPannel(Maths::Vec2f size, float slotCount, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f OffSet);
	void CreatInventoryNoPannel(Maths::Vec2f size, Maths::Vec2f rows, ECS::EntityID parent, ECS::e_AnchorSettings Anchor, Maths::Vec2f OffSet);

	//display controlls
	void LoadInventory(std::vector<ECS::UIPanel*> *PannleToUse, bool ToFit);
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

	ECS::EntityID GetSpecialSlot(std::string Name);


	std::vector <InventoryStoreData>* GetNullSlotData() { return &NullSlotData; }
	ECS::UIPanel* GetInventorySpace() { return InventorySpace; }
	std::string GetName() { return ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos)->Name; }
	bool GetIsDisplay() { return ECS::EntityComponentSystem::Instance()->GetComponent<InventoryComponent>(IveID, invetoryPos)->isDisplay; }
private:



private:

	//pannle created by this 
	ECS::UIPanel* InventorySpace;

	//invetory entity
	ECS::EntityID IveID;
	int invetoryPos = 0;

	//slots to remove
	std::vector <InventoryStoreData> NullSlotData;
};

