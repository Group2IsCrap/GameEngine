#include "CraftingRecipe.h"

#include "../Inventory/InventoryFunctionsGlobal.h"
#include "ItemDatabase.h"

CraftingRecipe::CraftingRecipe(int itemToMake, int countMaking, const std::vector<ItemRequirement>& requiredItems) :
    m_itemToMake(itemToMake),
    m_countMaking(countMaking),
    m_requiredItems(requiredItems)
{
}

CraftingRecipe::~CraftingRecipe()
{
}

bool CraftingRecipe::CanCraft(const std::string& inventoryGroup) const
{
    for (auto& requiredItem : m_requiredItems)
    {
        if (InventorySystem::GlobalFunctions::GetItemTypeTotal(inventoryGroup, "MainInventory", requiredItem.m_itemId) < requiredItem.m_numRequired)
        {
            return false;
        }
    }
    
    return true;
}

void CraftingRecipe::Craft(const std::string& inventoryGroup) const
{
    for (auto& requiredItem : m_requiredItems)
    {
        InventorySystem::GlobalFunctions::RemoveItemType(inventoryGroup, "MainInventory", requiredItem.m_numRequired, requiredItem.m_itemId);
    }

    for (int itemIndex = 0; itemIndex < m_countMaking; ++itemIndex)
    {
        InventorySystem::GlobalFunctions::AddItem(inventoryGroup, "MainInventory", ItemDatabase::Instance()->CreateInstanceOfItem(m_itemToMake));
    }
}

int CraftingRecipe::GetItemToMake() const
{
    return m_itemToMake;
}

int CraftingRecipe::GetCountMaking() const
{
    return m_countMaking;
}

const std::vector<CraftingRecipe::ItemRequirement>& CraftingRecipe::GetRequiredItems() const
{
    return m_requiredItems;
}
