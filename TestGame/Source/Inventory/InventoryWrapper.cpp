#include "InventoryWrapper.h"

InventoryEntity::InventoryEntity()
{
    AddComponent<InventoryComponent>();
    AddComponent<InventoryComponentGroupID>();

}

InventoryEntity::~InventoryEntity()
{
}

void InventoryEntity::SetGroup(InventoryComponentGroupID group)
{
    GetComponent<InventoryComponentGroupID>()->group = group.group;
    GetComponent<InventoryComponentGroupID>()->isDisplayButtons = group.isDisplayButtons;
    GetComponent<InventoryComponentGroupID>()->isDisplayAll = group.isDisplayAll;
}
