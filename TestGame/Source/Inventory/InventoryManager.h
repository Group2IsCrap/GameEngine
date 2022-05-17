#pragma once
#include"Inventory.h"
#include"Source/ECS/EntityWrappers/UIButton.h"
#include "Source/Graphics/AssetManager.h"
#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"

#include<Source/ECS/Systems/System.h>

typedef std::string GroupName;
inline ECS::EntityID ParentID;
class InventoryManager:public Firelight::Events::Listener , public Firelight::ECS::System
{
public:
	InventoryManager();
	~InventoryManager();
	
	void HandleEvents(const char* event, void* data);

	void CreateNewInventory();
	void RemoveInventory();


	void ItemChangeInventory();
	void GroupLoadOrUnload(std::string group);

	//creation
	void CreateInventory(GroupName group,std::string InvName, Maths::Vec2f size, Maths::Vec2f columnRows, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc);
	void CreateInventory(std::string group, std::string InvName, Maths::Vec2f size, unsigned int slotCont, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc);

	//render on screen
	void LoadInventory(GroupName group, std::string name);
	void LoadInventoryGroup(std::string group);

	//unloade invetory
	void UnloadInventory(GroupName group,std::string name);
	void UnloadInventoryGroup(std::string group);

	//Add to inventory
	void AddItem(GroupName group, std::string name, Firelight::ECS::Entity* item);
	void AddItem(GroupName group, std::string name, Firelight::ECS::EntityID item);

	//remove From Inventory
	void RemoveItem(GroupName group, std::string name, Firelight::ECS::Entity* item);
	void RemoveItem(GroupName group, std::string name, Firelight::ECS::EntityID item);

	void RemoveItem(GroupName group, std::string name, int item, int howMany);

	//Get item
	std::vector<ECS::EntityID> GetItems(GroupName group, std::string name, int item, int howMany);

	//find Item
	bool CheckInventory(ECS::EntityID ID,std::string InvName, GroupName group = "Null");

	
private:

	//resusable Entitys
	std::vector<ECS::UIPanel*> EntityIDPannlSlot;
	std::vector<ECS::EntityID> EntityIDButtion;

	//inv list
	std::map<GroupName, std::vector<Inventory*>> m_inventory;

};


