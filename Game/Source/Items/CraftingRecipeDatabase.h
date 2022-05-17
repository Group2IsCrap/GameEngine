#pragma once
#include <string>
#include <vector>

#include "CraftingRecipe.h"

class CraftingRecipeDatabase
{
public:
    ~CraftingRecipeDatabase();

    static CraftingRecipeDatabase& Instance();

    void LoadCraftingRecipes(const std::string& filepath);

    const std::vector<const CraftingRecipe*>& GetAllCraftingRecipes() const;
    const CraftingRecipe*                     GetCraftingRecipeForItem(int itemId) const;

private:
    CraftingRecipeDatabase();

    std::vector<const CraftingRecipe*> m_recipes;
};
