#include "InventoryEntity.h"
#include"Source/Events/EventDispatcher.h"
#include"InventoryEvents.h"
#include <Source/Graphics/AssetManager.h>

InventoryEntity::InventoryEntity() : Entity()
{
    AddComponent<IdentificationComponent>()->name = "Inventory";
    AddComponent<InventoryComponentGroupID>();

}

InventoryEntity::InventoryEntity(std::string name, bool isDisplayButtons, bool isDisplayAll, Firelight::Keys keyToActivate) : InventoryEntity()
{

    this->GetComponent<IdentificationComponent>()->name = name;

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

void InventoryEntity::AddInventory(std::string name, std::string backgroundTexture, int rowCount, int columnCount, Firelight::Maths::Vec2f size, Firelight::Maths::Vec2f border, Firelight::Maths::Vec2f slotMargin, Firelight::Maths::Vec2f offset, Firelight::ECS::e_AnchorSettings UIAnchor)
{
    AddInventory(name, backgroundTexture, rowCount, columnCount, size, Firelight::Maths::Vec2f(0.0f, 0.0f), border, slotMargin, offset, UIAnchor);
}

void InventoryEntity::AddInventory(std::string name, std::string backgroundTexture, int rowCount, int columnCount, Firelight::Maths::Vec2f size, Firelight::Maths::Vec2f slotSize, Firelight::Maths::Vec2f border, Firelight::Maths::Vec2f slotMargin, Firelight::Maths::Vec2f offset, Firelight::ECS::e_AnchorSettings UIAnchor)
{
    InventoryComponent* inventoryComponent = AddComponent<InventoryComponent>();
    inventoryComponent->backgroundTexture = backgroundTexture;
    AddComponent<InventoryComponentButtonLayout>();
    AddComponent<InventoryComponentOutPut>();
    AddComponent<InventoryComponentInPut>();

    GetInventoryGroup()->numberOfInventories++;

    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->name = name;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->offset = offset;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->size = size;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->rowCount = rowCount;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->columnCount = columnCount;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->anchorSettings = UIAnchor;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->slotCount = rowCount * columnCount;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->slotStartPositon = GetInventoryGroup()->totalNumberOfSlots;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->slotSize = slotSize;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->slotMargin = slotMargin;
    this->GetInventoryComponent(GetInventoryGroup()->numberOfInventories - 1)->margin = border;

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
        if (GetInventoryComponent(static_cast<int>(i))->name != name)
        {
            continue;
        }

        this->RemoveComponent<InventoryComponent>(static_cast<int>(i));
        this->RemoveComponent<InventoryComponentButtonLayout>(static_cast<int>(i));
    }

    std::vector<std::string> data;
    data.push_back(GetInventoryGroup()->group);
    data.push_back(name);
    Firelight::Events::EventDispatcher::InvokeListeners<Firelight::Events::Inventory::RemoveInventory>((void*)&data);

}

void InventoryEntity::AddOutputCommands(int invetoryNum, std::function< void(void*) >&& callbackFunction)
{
    GetComponent<InventoryComponentOutPut>(invetoryNum)->outputCommand.push_back(callbackFunction);
}

void InventoryEntity::AddKeyCommands(int invetoryNum, Firelight::Keys actionKey,std::string actionName, std::string show)
{
    for (size_t i = GetInventoryComponent(invetoryNum)->slotStartPositon; i < GetInventoryComponent(invetoryNum)->slotStartPositon + (int)GetInventoryComponent(invetoryNum)->slotCount; i++)
    {
        if (GetSlot(static_cast<int>(i))->ActionIndex != -1) {
            continue;
        }
        else
        {
            InventoryComponentKeyPressAction* Action = AddComponent<InventoryComponentKeyPressAction>();;
           
            Action->ActionName = actionName;
            Action->keyToActivate = actionKey;
            Action->DisplayText = show;
            GetSlot(static_cast<int>(i))->ActionIndex = static_cast<int>(GetComponents<InventoryComponentKeyPressAction>().size() - 1);

            break;
        }


    }
   


}

void InventoryEntity::AddSpecialSlot(int InventoryNumber, std::string slotName, std::string slotTexture, Firelight::Maths::Vec2f offset, Firelight::Maths::Vec2f size, Firelight::ECS::e_AnchorSettings anchorSettings, std::vector<std::string> tags)
{
    for (size_t i = GetInventoryComponent(InventoryNumber)->slotStartPositon; i < GetInventoryComponent(InventoryNumber)->slotStartPositon + (int)GetInventoryComponent(InventoryNumber)->slotCount; i++)
    {
        if (GetSlot(static_cast<int>(i))->specialSlotIndex != -1) {
            continue;
        }
        else
        {
            InventoryComponentSpecialSlot* Slot = AddComponent<InventoryComponentSpecialSlot>();;
            Slot->slotTexture = Firelight::Graphics::AssetManager::Instance().GetTexture(slotTexture);
            if (Slot->slotTexture == nullptr)
            {
                Slot->slotTexture = Firelight::Graphics::AssetManager::Instance().GetDefaultTexture();
            }
            Slot->anchorSettings = anchorSettings;
            Slot->offset = offset;
            Slot->slotName = slotName;
            Slot->size = size;
            Slot->tags = tags;
            
            GetSlot(static_cast<int>(i))->specialSlotIndex = static_cast<int>(GetComponents<InventoryComponentSpecialSlot>().size() - 1);

            break;
        }


    }
}
