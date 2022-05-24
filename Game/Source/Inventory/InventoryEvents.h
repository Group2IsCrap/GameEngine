#pragma once
#include "Source/Events/Event.h"
namespace Firelight::Events::Inventory
{
	NEW_EVENT(UpdateInventory);
	NEW_EVENT(AddNewInventory);
	NEW_EVENT(RemoveInventory);


	NEW_EVENT(AddItem);
	NEW_EVENT(FindItem);

	NEW_EVENT(RemoveItem);
	NEW_EVENT(RemoveItemType);

	NEW_EVENT(GetItemType);
	NEW_EVENT(GetItemTypeNumber);
	NEW_EVENT(GetSpecialSlot);
	NEW_EVENT(DropAll);

	NEW_EVENT(LoadInventoryGroup);
	NEW_EVENT(UnloadInventoryGroup);
	
}