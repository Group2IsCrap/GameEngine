#include "CraftingRecipe.h"

CraftingRecipe::CraftingRecipe(int itemToMake, const std::vector<ItemRequirement>& requiredItems) :
    m_itemToMake(itemToMake),
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

const std::vector<CraftingRecipe::ItemRequirement>& CraftingRecipe::GetRequiredItems() const
{
    m_requiredItems;
}
