#pragma once
#include <Source/ECS/EntityWrappers/Entity.h>
#include"Source/ECS/EntityWrappers/UIButton.h"
#include "Source/Graphics/AssetManager.h"

#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"

#include<string>
namespace InventorySystem::GlobalFunctions {

      /// <summary>
      /// control Inventory
      /// allows for esay return of data 
      /// true = bad
      /// </summary>

    struct AddOrRemoveStruct
    {
        std::string groupName;
        std::string inventoryName;
        Firelight::ECS::EntityID itemID;
        bool returnData;
    };

    inline bool AddItem(std::string group, std::string name,Firelight::ECS::EntityID item) 
    {
        AddOrRemoveStruct addItemData;
        addItemData.groupName = group;
        addItemData.inventoryName = name;
        addItemData.itemID = item;

        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inventory::AddItem::sm_descriptor, &addItemData);

        return addItemData.returnData;
    }
    inline bool AddItem(std::string group, std::string name,Firelight::ECS::Entity* item) 
    {
        return AddItem(group,name,item->GetEntityID());
    }
   

    inline bool RemoveItem(std::string group, std::string name, Firelight::ECS::EntityID item) 
    {
        AddOrRemoveStruct removeItemData;
        removeItemData.groupName = group;
        removeItemData.inventoryName = name;
        removeItemData.itemID = item;

        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inventory::RemoveItem::sm_descriptor, &removeItemData);

        return removeItemData.returnData;
    }

    inline bool RemoveItem(std::string group, std::string name, Firelight::ECS::Entity* item) 
    {
        return RemoveItem(group, name, item->GetEntityID());
    }



    struct RemoveItemTypeData
    {
        std::string groupName;
        std::string inventoryName;
        int Number;
        int Type;
        bool returnData;
    };

    inline bool RemoveItemType(std::string group, std::string name,int howMany, int type) 
    {
        RemoveItemTypeData removeItemTypeData;
        removeItemTypeData.groupName = group;
        removeItemTypeData.inventoryName = name;
        removeItemTypeData.Number = howMany;
        removeItemTypeData.Type = type;
        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inventory::RemoveItemType::sm_descriptor, &removeItemTypeData);

        return removeItemTypeData.returnData;
    }
    
    struct GetItemTypeData
    {
        std::string groupName;
        std::string inventoryName;
        int Number;
        int Type;
        bool isRemoveData;
        std::vector<Firelight::ECS::EntityID> returnData;
    };

    inline std::vector<Firelight::ECS::EntityID> GetItemType(std::string group, std::string name,int howMany, int type) 
    {
        GetItemTypeData getItemTypeData;
        getItemTypeData.groupName = group;
        getItemTypeData.inventoryName = name;
        getItemTypeData.Number = howMany;
        getItemTypeData.Type = type;

        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inventory::GetItemType::sm_descriptor, &getItemTypeData);

        return getItemTypeData.returnData;
    }

    struct ItemTotalData
    {
        std::string groupName;
        std::string inventoryName; 
        int type;

        int returnData;
    };

    inline int GetItemTypeTotal(std::string group, std::string name,int type) 
    {
        ItemTotalData itemTotalData;
        itemTotalData.groupName = group;
        itemTotalData.inventoryName = name;
        itemTotalData.type = type;
        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inventory::GetItemTypeNumber::sm_descriptor, &itemTotalData);

        return itemTotalData.returnData;
    }

    struct SpecialItemSlotData
    {
        std::string groupName;
        std::string inventoryName;
        std::string slotName;

        Firelight::ECS::EntityID returnData;
    };

    inline Firelight::ECS::EntityID GetSpecialSlotEntity(std::string group, std::string name, std::string slotName)
    {
        SpecialItemSlotData specialItemSlotData;
        specialItemSlotData.groupName = group;
        specialItemSlotData.inventoryName = name;
        specialItemSlotData.slotName = slotName;
        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inventory::GetSpecialSlot::sm_descriptor, &specialItemSlotData);

        return specialItemSlotData.returnData;
    }

    struct RemoveInvetoryData
    {
        std::string groupName;
        std::string inventoryName;
        

        Firelight::ECS::EntityID returnData;
    };

    inline void RemoveInventory(std::string group, std::string name)
    {
        RemoveInvetoryData removeInvetoryData;
        removeInvetoryData.groupName = group;
        removeInvetoryData.inventoryName = name;
       
        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inventory::RemoveInventory::sm_descriptor, &removeInvetoryData);

        //return specialItemSlotData.returnData;
    }


    inline void RemoveAllItems(std::string group, std::string name) {
        RemoveInvetoryData removeInvetoryData;
        removeInvetoryData.groupName = group;
        removeInvetoryData.inventoryName = name;
        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inventory::DropAll::sm_descriptor, &removeInvetoryData);
    }

 }