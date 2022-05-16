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

    void AddInventory(std::string name, int rowCount, int columnCount, Firelight::Maths::Vec2f size, Firelight::Maths::Vec2f offset, Firelight::ECS::e_AnchorSettings UIAnchor);
    void RemoveInventory(std::string name);
    void AddInputCommands();
    void AddOutputCommands();

    void AddSpecialSlot(std::string slotName, Firelight::Maths::Vec2f offset, Firelight::ECS::e_AnchorSettings anchorSettings, std::vector<std::string> tags);
    void AddButionLayout(Firelight::Maths::Vec2f offSet, Firelight::ECS::e_AnchorSettings anchorSettings, bool isActive);

    InventoryComponentGroupID* GetInventoryGroup() { return GetComponent<InventoryComponentGroupID>(); }
    InventoryComponent* GetInventoryComponent(int number) { return GetComponent<InventoryComponent>(number); }
    

};

