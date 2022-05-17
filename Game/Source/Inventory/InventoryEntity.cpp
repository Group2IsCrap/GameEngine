#include "InventoryEntity.h"
#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"

InventoryEntity::InventoryEntity()
{
    AddComponent<InventoryComponentGroupID>(); 
}

InventoryEntity::InventoryEntity(std::string name, bool isDisplayButtons, bool isDisplayAll, Firelight::Keys keyToActivate)
{
    AddComponent<InventoryComponentGroupID>();
    
    GetInventoryGroup()->group = name;
    GetInventoryGroup()->isDisplayButtons = isDisplayButtons;
    GetInventoryGroup()->isDisplayAll = isDisplayAll;
    GetInventoryGroup()->keyToActivate = keyToActivate;
    //add inv
    Firelight::Events::EventDispatcher::InvokeFunctions <Firelight::Events::Inventory::AddNewInventory>();
    
}

InventoryEntity::~InventoryEntity()
{
}

void InventoryEntity::AddInventory(std::string name, int rowCount, int columnCount, Firelight::Maths::Vec2f size, Firelight::Maths::Vec2f offset, Firelight::ECS::e_AnchorSettings UIAnchor)
{
    AddComponent<InventoryComponent>();
    AddComponent<InventoryComponentButtonLayout>();
    GetInventoryGroup()->numberOfInventories++;

    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->name = name;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->offset = offset;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->size = size;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->rowCount = rowCount;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->columnCount = columnCount;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->anchorSettings = UIAnchor;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->slotCount = rowCount * columnCount;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->slotStartPositon = GetInventoryGroup()->totalNumberOfSlots;

    int index = GetInventoryGroup()->totalNumberOfSlots;
    for (size_t i = 0; i < rowCount; i++)
    {
        for (size_t i = 0; i < columnCount; i++)
        {
            AddComponent<InventorySlots>();
            AddComponent<InventoryStoreData>();

            //pair up
            GetComponent<InventorySlots>(index)->currentIndex = index;
            GetComponent<InventoryStoreData>(index)->slotIndex = index;
            index++;
            
        }

    }
    GetInventoryGroup()->totalNumberOfSlots = index;
    //add inv
    Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::Inventory::AddNewInventory>();
}

void InventoryEntity::RemoveInventory(std::string name)
{
    for (size_t i = 0; i < GetInventoryGroup()->numberOfInventories; i++)
    {
        if (GetInventoryComponent(i)->name != name)
        {
            continue;
        }

        this->RemoveComponent<InventoryComponent>(i);
        this->RemoveComponent<InventoryComponentButtonLayout>(i);
    }

    std::vector<std::string> data;
    data.push_back(GetInventoryGroup()->group);
    data.push_back(name);
    Firelight::Events::EventDispatcher::InvokeListeners<Firelight::Events::Inventory::RemoveInventory>((void*)&data);

}

void InventoryEntity::AddSpecialSlot(int InventoryNumber, std::string slotName, Firelight::Maths::Vec2f offset, Firelight::Maths::Vec2f size, Firelight::ECS::e_AnchorSettings anchorSettings, std::vector<std::string> tags)
{
    AddComponent<InventoryComponentSpecialSlot>();
    
    for (size_t i = GetInventoryComponent(InventoryNumber)->slotStartPositon; i < GetInventoryComponent(InventoryNumber)->slotStartPositon + (int)GetInventoryComponent(InventoryNumber)->slotCount; i++)
    {
        if (GetSlot(i)->specialSlotIndex != -1) {
            continue;
        }
        else
        {
            InventoryComponentSpecialSlot* Slot = GetComponents<InventoryComponentSpecialSlot>().back();
            Slot->anchorSettings = anchorSettings;
            Slot->offset = offset;
            Slot->slotName = slotName;
            Slot->size = size;
            GetSlot(i)->specialSlotIndex = GetComponents<InventoryComponentSpecialSlot>().size() - 1;

            break;
        }


    }
}
