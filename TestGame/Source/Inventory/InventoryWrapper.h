#pragma once
#include<Source/ECS/EntityWrappers/Entity.h>
#include"InventoryComponents.h"
class InventoryWrapper:public Firelight::ECS::Entity
{
public:
    InventoryWrapper();
    ~InventoryWrapper();

    void SetGroup(std::string Group);
    void SetGroup(InventoryComponentGroupID Group);

    InventoryComponentGroupID* GetInvGroup() { return GetComponent<InventoryComponentGroupID>(); }
    InventoryComponent* GetInvComp() { return GetComponent<InventoryComponent>(); }
    

};

