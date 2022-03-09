#pragma once

#include <Source/ECS/Components/UIComponents.h>
#include <Source/ECS/EntityWrappers/UIEntity.h>
#include <Source/ECS/EntityWrappers/UIPanel.h>
#include <Source/ECS/Systems/UISystem.h>
#include<Source/Maths/Vec2.h>
#include <string>
#include <vector>

#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"

using namespace Firelight;

struct SlotInfo
{	
	int CurrPos = 0;
	ECS::EntityID SlotID= NULL;
	bool IsUsed = false;
};
struct SlotData
{
	SlotInfo* CurrSlot;
	int StackSize = -1;
	std::vector<ECS::EntityID> EntityIDs;
	ECS::EntityID UITexID;
};
class Inventory
{
public:
	Inventory();
	Inventory(std::string Name);
	~Inventory();


	void CreatInventoryNoPannel(Maths::Vec2f size, float slotCount, ECS::Entity* parent);
	void CreatInventoryNoPannel(Maths::Vec2f size, Maths::Vec2f rows, ECS::Entity* parent);

	//inventory without manager
	void CreatInventory(Maths::Vec2f size, float slotCount,ECS::Entity* parent);
	void CreatInventory(Maths::Vec2f size, Maths::Vec2f rows, ECS::Entity* parent);

	//display controlls
	void LoadInventory(std::vector<ECS::UIPanel*> *PannleToUse, bool ToFit);
	void UnloadInventory();

	//item controlls
	bool AddItem(Firelight::ECS::Entity* item);
	bool AddItem(Firelight::ECS::EntityID item);
	bool AddItem(SlotData* item);

	void RemoveItem(Firelight::ECS::Entity* item);
	void RemoveItem(Firelight::ECS::EntityID item);

	bool FindItem(Firelight::ECS::Entity* item);
	bool FindItem(Firelight::ECS::EntityID item);

	bool RemoveItemType(int howMany, int type);
	int GetItemTypeTotal(int type);
	std::vector<ECS::EntityID> GetItemType(int howMany, int type);

	//slot controlls
	void AddSlot();
	void RemoveSlot(SlotData* item);
	SlotInfo* FindSlot(int Number);

	void Place(SlotData* slotData);


	std::vector <SlotData*>* GetNullSlotData() { return &NullSlotData; }
	ECS::UIPanel* GetInventorySpace() { return InventorySpace; }
	std::string GetName() { return m_Name; }
	bool GetIsDisplay() { return isDisplay; }
private:



private:

	ECS::UIPanel* InventorySpace;
	std::string m_Name;
	UINT SlotCount = 0;
	int RowCount = 0;
	int ColoumCount = 0;


	bool isOutput;
	bool isInput;
	bool isSwap;

	bool isDisplay = false;

	Maths::Vec2i Size;
	std::vector<std::pair<SlotInfo, SlotData*>> Grid;

	std::vector <SlotData*> NullSlotData;
};

