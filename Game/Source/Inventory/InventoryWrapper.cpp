#include "InventoryWrapper.h"

InventoryWrapper::InventoryWrapper()
{
    AddComponent<InventoryComponent>();
    AddComponent<InventoryComponentGroupID>();

}

InventoryWrapper::~InventoryWrapper()
{
}

void InventoryWrapper::SetGroup(InventoryComponentGroupID Group)
{
    GetComponent<InventoryComponentGroupID>()->Group = Group.Group;
    GetComponent<InventoryComponentGroupID>()->isDisplayButtions = Group.isDisplayButtions;
    GetComponent<InventoryComponentGroupID>()->isDisplayAll = Group.isDisplayAll;
}
