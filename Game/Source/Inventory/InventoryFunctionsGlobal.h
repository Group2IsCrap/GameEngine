#pragma once
#include <Source/ECS/EntityWrappers/Entity.h>
#include"Source/ECS/EntityWrappers/UIButton.h"
#include "Source/Graphics/AssetManager.h"

#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"

#include<string>
namespace InventorySystem::Global_Functions {

      /// <summary>
      /// control Inventory
      /// allows for esay return of data 
      /// true = bad
      /// </summary>

      struct AddOrRemoveStruct
    {
        std::string groupName;
        std::string InventoryName;
        Firelight::ECS::EntityID itemID;
        bool returnData;
    };

    inline bool AddItem(std::string group, std::string Name,Firelight::ECS::EntityID item) {
        AddOrRemoveStruct AddItemData;
        AddItemData.groupName = group;
        AddItemData.InventoryName = Name;
        AddItemData.itemID = item;

        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inv::ADD_ITEM::sm_descriptor, &AddItemData);

        return AddItemData.returnData;
    }
    inline bool AddItem(std::string group, std::string Name,Firelight::ECS::Entity* item) {
        return AddItem(group,Name,item->GetEntityID());
    }
   

     inline bool RemoveItem(std::string group, std::string Name, Firelight::ECS::EntityID item) {
        AddOrRemoveStruct RemoveItemData;
        RemoveItemData.groupName = group;
        RemoveItemData.InventoryName = Name;
        RemoveItemData.itemID = item;

        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inv::REMOVE_ITEM::sm_descriptor, &RemoveItemData);

        return RemoveItemData.returnData;
    }
    inline bool RemoveItem(std::string group, std::string Name, Firelight::ECS::Entity* item) {
        return RemoveItem(group, Name, item->GetEntityID());
    }



    struct RemoveItemTypeData
    {
        std::string groupName;
        std::string InventoryName;
        int Number;
        int Type;
        bool returnData;
    };
    inline bool RemoveItemType(std::string group, std::string Name,int howMany, int type) {
        RemoveItemTypeData RemoveItemTypeData;
        RemoveItemTypeData.groupName = group;
        RemoveItemTypeData.InventoryName = Name;
        RemoveItemTypeData.Number = howMany;
        RemoveItemTypeData.Type = type;
        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inv::REMOVE_ITEM_TYPE::sm_descriptor, &RemoveItemTypeData);

        return RemoveItemTypeData.returnData;
    }
    
    struct GetItemTypeData
    {
        std::string groupName;
        std::string InventoryName;
        int Number;
        int Type;
        bool isRemoveData;
        std::vector<Firelight::ECS::EntityID> returnData;
    };

    inline std::vector<Firelight::ECS::EntityID> GetItemType(std::string group, std::string Name,int howMany, int type) {
        GetItemTypeData GetItemTypeData;
        GetItemTypeData.groupName = group;
        GetItemTypeData.InventoryName = Name;
        GetItemTypeData.Number = howMany;
        GetItemTypeData.Type = type;

        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inv::GET_ITEM_TYPE::sm_descriptor, &GetItemTypeData);

        return GetItemTypeData.returnData;
    }

    struct ItemTotalData
    {
        std::string groupName;
        std::string InventoryName; 
        int type;

        int ReturnData;
    };

    inline int GetItemTypeTotal(std::string group, std::string Name,int type) {


        ItemTotalData ItemTotalData;
        ItemTotalData.groupName = group;
        ItemTotalData.InventoryName = Name;
        ItemTotalData.type = type;
        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inv::GET_ITEM_TYPE_NUMBER::sm_descriptor, &ItemTotalData);

        return ItemTotalData.ReturnData;
    }

    struct SpecilaItemSlotData
    {
        std::string groupName;
        std::string InventoryName;
        std::string SlotName;

        Firelight::ECS::EntityID ReturnData;
    };
    inline Firelight::ECS::EntityID GetSpecilaSlotEntity(std::string group, std::string Name, std::string SlotName) {
        SpecilaItemSlotData SpecilaItemSlotData;
        SpecilaItemSlotData.groupName = group;
        SpecilaItemSlotData.InventoryName = Name;
        SpecilaItemSlotData.SlotName = SlotName;
        Firelight::Events::EventDispatcher::InvokeListeners(Firelight::Events::Inv::GET_SPECIAL_SLOT::sm_descriptor, &SpecilaItemSlotData);

        return SpecilaItemSlotData.ReturnData;
    }



 }