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

    InventoryComponentGroupID* GetInvGroup() { return GetComponent<InventoryComponentGroupID>(); }
    InventoryComponent* GetInvComp() { return GetComponent<InventoryComponent>(); }
    
    //inventory controlls
    bool FindItem(int ItemID);
    std::vector<Firelight::ECS::EntityID> GetItem();
    bool RemoveItem();
    bool AddItem();
};

