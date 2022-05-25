#pragma once
#include<Source/ECS/EntityWrappers/Entity.h>
#include"InventoryComponents.h"
#include"Source/Maths/Vec2.h"
class InventoryEntity : public Firelight::ECS::Entity
{
public:
    InventoryEntity();
    InventoryEntity(std::string name, bool isDisplayButtons, bool isDisplayAll, Firelight::Keys keyToActivate);
    ~InventoryEntity();

    void AddInventory(std::string name, std::string backgroundTexture, int rowCount, int columnCount, Firelight::Maths::Vec2f size, Firelight::Maths::Vec2f border, Firelight::Maths::Vec2f slotMargin, Firelight::Maths::Vec2f offset, Firelight::ECS::e_AnchorSettings UIAnchor);
    void AddInventory(std::string name, std::string backgroundTexture, int rowCount, int columnCount, Firelight::Maths::Vec2f size, Firelight::Maths::Vec2f slotSize, Firelight::Maths::Vec2f border, Firelight::Maths::Vec2f slotMargin, Firelight::Maths::Vec2f offset, Firelight::ECS::e_AnchorSettings UIAnchor);
    void RemoveInventory(std::string name);
    void AddOutputCommands(int invetoryNum, std::function< void(void*) >&& callbackFunction);
    void AddInputCommands(int invetoryNum, std::function< void(void*) >&& callbackFunction);

    void AddKeyCommands(int invetoryNum,Firelight::Keys actionKey, std::string actionName, std::string show);
    

    void AddSpecialSlot(int InventoryNumber,std::string slotName, std::string slotTexture, Firelight::Maths::Vec2f offset, Firelight::Maths::Vec2f size, Firelight::ECS::e_AnchorSettings anchorSettings, std::vector<std::string> tags);

    InventoryComponentGroupID* GetInventoryGroup() { return GetComponent<InventoryComponentGroupID>(); }
    InventoryComponent* GetInventoryComponent(int number) { return GetComponent<InventoryComponent>(number); }
    InventoryComponentButtonLayout* GetButtonLayout(int number) { return GetComponent<InventoryComponentButtonLayout>(number); }
    InventoryStoreData* GetSlotData(int number){ return GetComponent<InventoryStoreData>(number); }
    InventorySlots* GetSlot(int number) { return GetComponent<InventorySlots>(number); }
};

