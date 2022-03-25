#include "InventoryWrapper.h"

InventoryWrapper::InventoryWrapper()
{
    AddComponent<InventoryComponent>();
    AddComponent<InventoryComponentGroupID>();

}
