#pragma once
#include"Inventory.h"


typedef std::string GroupName;
class InventoryManager
{
public:
	InventoryManager();
	~InventoryManager();
	
	void ItemChangeInventory();

	//creation
	void CreatInventory(GroupName group,std::string InvName, Maths::Vec2f size, Maths::Vec2f columnRows, ECS::Entity* parent);
	void CreatInventory(std::string group, std::string InvName, Maths::Vec2f size, unsigned int slotCont, ECS::Entity* parent);

	//render on screen
	void LoadInventory(GroupName group, std::string name);
	void LoadInventoryGroup(std::string group);

	//unloade invetory
	void UnloadInventory(GroupName group,std::string name);
	void UnloadInventoryGroup(std::string group);

	//Add to inventory
	void AddItem(GroupName group, std::string Name, Firelight::ECS::Entity* item);
	void AddItem(GroupName group, std::string Name, Firelight::ECS::EntityID item);

	//remove From Inventory
	void RemoveItem(GroupName group, std::string Name, Firelight::ECS::Entity* item);
	void RemoveItem(GroupName group, std::string Name, Firelight::ECS::EntityID item);

	void RemoveItem(GroupName group, std::string Name, int item, int howMany);

	//Get item
	std::vector<ECS::EntityID> GetItems(GroupName group, std::string Name, int item, int howMany);

	//find Item
	bool CheckInventory(ECS::EntityID ID,std::string InvName, GroupName Group = "Null");

private:

	//resusable Entitys
	std::vector<ECS::UIPanel*> EntityIDPannlSlot;
	std::vector<ECS::EntityID> EntityIDButtion;

	//inv list
	std::map<GroupName, std::vector<Inventory*>> m_Inventory;
};

//TODO Change to templates