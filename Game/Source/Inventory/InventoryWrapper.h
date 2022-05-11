#pragma once
#include<Source/ECS/EntityWrappers/Entity.h>
#include"InventoryComponents.h"
#include"Source/Maths/Vec2.h"
class InventoryWrapper:public Firelight::ECS::Entity
{
public:
    InventoryWrapper();
    InventoryWrapper(std::string Name , int RowCount, int ColoumCount, Firelight::Maths::Vec2f Size, Firelight::Maths::Vec2f OffSet, Firelight::ECS::e_AnchorSettings UIAnchor);
    ~InventoryWrapper();

    void SetGroup(InventoryComponentGroupID Group);
    void SetGroup(InventoryComponentGroupID* Group);

    void AddInputCommands();
    void AddOutputCommands();

    void AddSpecialSlot(std::string SlotName, Firelight::Maths::Vec2f OffSet, Firelight::ECS::e_AnchorSettings AnchorSettings, std::vector<std::string> Tags);
    void AddButionLayout(Firelight::Maths::Vec2f offSet, Firelight::ECS::e_AnchorSettings anchorSettings, bool isActive);

    InventoryComponentGroupID* GetInvGroup() { return GetComponent<InventoryComponentGroupID>(); }
    InventoryComponent* GetInvComp() { return GetComponent<InventoryComponent>(); }
    

};

