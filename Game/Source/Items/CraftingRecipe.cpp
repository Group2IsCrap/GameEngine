#include "CraftingRecipe.h"

#include "../Inventory/InventoryFunctionsGlobal.h"

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
        if (InventorySystem::GlobalFunctions::GetItemTypeTotal(inventoryGroup, "MainIven", requiredItem.m_itemId) < requiredItem.m_numRequired)
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
        InventorySystem::GlobalFunctions::RemoveItemType(inventoryGroup, "MainIven", requiredItem.m_numRequired, requiredItem.m_itemId);
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
