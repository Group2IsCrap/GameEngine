#pragma once
#include "Source/Events/Event.h"
namespace Firelight::Events::Inventory
{
	NEW_EVENT(UpdateInventory);
	NEW_EVENT(AddNewInventory);

	NEW_EVENT(RemoveItem);
	NEW_EVENT(AddItem);
	NEW_EVENT(FindItem);

	NEW_EVENT(LoadInventoryGroup);
	NEW_EVENT(UnloadInventoryGroup);



	
}