#pragma once
#include "Inventory.h"
#include "Source/ECS/EntityWrappers/UIButton.h"
#include "Source/Graphics/AssetManager.h"
#include "Source/Events/EventDispatcher.h"
#include "InventoryEvents.h"

#include<Source/ECS/Systems/System.h>


namespace InventorySystem
{

	typedef std::string GroupName;
	inline ECS::EntityID UIParentID;

	class InventoryManager : public Firelight::Events::Listener, public Firelight::ECS::System
	{
	public:
		InventoryManager();
		~InventoryManager();

		void HandleEvents(const char* event, void* data);

		void CreateNewInventory();
		void RemoveInventory(std::string groupName, std::string inventoryName);


		void ItemChangeInventory();

		//creation
		void CreateInventory(GroupName group, std::string InvName, Maths::Vec2f size, Maths::Vec2f columnRows, Maths::Vec2f margin, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc);
		void CreateInventory(std::string group, std::string InvName, Maths::Vec2f size, unsigned int slotCount, Maths::Vec2f margin, ECS::EntityID parent, Maths::Vec2f offSet, ECS::e_AnchorSettings anc);

		void GroupLoadOrUnload(std::string group);
		//render on screen
		void LoadInventory(GroupName group, std::string name);
		void LoadInventoryGroup(std::string group);

		//unloade invetory
		void UnloadInventory(GroupName group, std::string name);
		void UnloadInventoryGroup(std::string group);

		//Add to inventory
		void AddItem(GroupName group, std::string name, Firelight::ECS::Entity* item);
		bool AddItem(GroupName group, std::string name, Firelight::ECS::EntityID item);

		//remove From Inventory
		bool RemoveItem(GroupName group, std::string name, Firelight::ECS::Entity* item);
		bool RemoveItem(GroupName group, std::string name, Firelight::ECS::EntityID item);
		bool RemoveItem(GroupName group, std::string name, int item, int howMany);

		//Get item
		std::vector<ECS::EntityID> GetItems(GroupName group, std::string name, int item, int howMany);

		//find Item
		bool CheckInventory(ECS::EntityID ID, std::string InvName, GroupName group = "Null");
	private:
		int GetNumberOfItemInInventory(GroupName group, std::string name, int item);
		ECS::EntityID GetSpecialSlot(GroupName group, std::string name, std::string slotName);
	private:

		//resusable Entitys
		std::vector<ECS::UIPanel*> m_entityIDPanelSlot;
		std::vector<ECS::EntityID> m_entityIDButton;

		//inv list
		std::map<GroupName, std::vector<Inventory*>> m_inventory;
		std::map<GroupName, bool> m_inventoryLoaded;
		std::map<GroupName, size_t> m_inventoryEventsIDs;
	};

}
