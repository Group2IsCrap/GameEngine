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

private:
    CraftingRecipeDatabase();

    std::vector<CraftingRecipe*> m_recipes;
};
