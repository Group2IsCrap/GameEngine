#pragma once

#include <vector>

class CraftingRecipe
{
public:
    struct ItemRequirement
    {
        int m_itemId;
        int m_numRequired;
    };

public:
    CraftingRecipe(int itemToMake, const std::vector<ItemRequirement>& requiredItems);
    ~CraftingRecipe();
    
    // TODO: Hookup to inventory when it's available
    
    //bool canCraft(const Inventory& invt);     // Check if the given inventory has the items to complete the recipe
    
    //void craft(Inventory& invt);              // Remove the required items from the given inventory and add the item made

    int                                 GetItemToMake() const;
    const std::vector<ItemRequirement>& GetRequiredItems() const;

private:
    int                          m_itemToMake;
    std::vector<ItemRequirement> m_requiredItems;
};

