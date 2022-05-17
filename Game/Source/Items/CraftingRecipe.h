#pragma once

#include <vector>
#include <string>

class CraftingRecipe
{
public:
    struct ItemRequirement
    {
        int m_itemId;
        int m_numRequired;
    };

public:
    CraftingRecipe(int itemToMake, int countMaking, const std::vector<ItemRequirement>& requiredItems);
    ~CraftingRecipe();
    
    // Check if the given inventory has the items to complete the recipe
    bool CanCraft(const std::string& inventoryGroup) const;
    
    // Remove the required items from the given inventory and add the item made
    void Craft(const std::string& inventoryGroup) const;

    int                                 GetItemToMake() const;
    int                                 GetCountMaking() const;
    const std::vector<ItemRequirement>& GetRequiredItems() const;

private:
    int                          m_itemToMake;
    int                          m_countMaking;
    std::vector<ItemRequirement> m_requiredItems;
};
