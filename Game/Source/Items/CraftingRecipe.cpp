#include "CraftingRecipe.h"

CraftingRecipe::CraftingRecipe(int itemToMake, int countMaking, const std::vector<ItemRequirement>& requiredItems) :
    m_itemToMake(itemToMake),
    m_countMaking(countMaking),
    m_requiredItems(requiredItems)
{
}

CraftingRecipe::~CraftingRecipe()
{
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
