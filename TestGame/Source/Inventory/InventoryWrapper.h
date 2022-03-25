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




};

class InventoryManagerWrapper :public Firelight::ECS::Entity
{
public:
    InventoryManagerWrapper();
    ~InventoryManagerWrapper();

    void SetGroup(std::string Group);
    void SetGroup(InventoryComponentGroupID Group);




};