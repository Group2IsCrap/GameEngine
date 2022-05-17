#pragma once
#include<Source/ECS/EntityWrappers/Entity.h>
#include"InventoryComponents.h"
class InventoryEntity:public Firelight::ECS::Entity
{
public:
    InventoryEntity();
    ~InventoryEntity();

    void SetGroup(std::string group);
    void SetGroup(InventoryComponentGroupID group);

    InventoryComponentGroupID* GetInventoryGroup() { return GetComponent<InventoryComponentGroupID>(); }
    InventoryComponent* GetInventoryComponent() { return GetComponent<InventoryComponent>(); }
    

};

