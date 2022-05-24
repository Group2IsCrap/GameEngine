#include "ImGuiPlayerLayer.h"

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"

#include "Source/ECS/EntityWrappers/GameEntity.h"
#include "Source/ECS/Components/BasicComponents.h"
#include "Source/ECS/Components/PhysicsComponents.h"

#include "Items/ItemDatabase.h"

#include "Inventory/InventoryEvents.h"

ImGuiPlayerLayer::ImGuiPlayerLayer() :
	m_debugCraftingMenuEnabled(false),
	m_craftingRecipes(nullptr)
{
	SetupTheme();
}

ImGuiPlayerLayer::~ImGuiPlayerLayer()
{

}

void ImGuiPlayerLayer::SetPlayer(Firelight::ECS::Entity* entity)
{
	this->playerEntity = entity;
}

void ImGuiPlayerLayer::Render()
{
	RenderInformation();
	RenderDebugCraftingMenu();
}

void ImGuiPlayerLayer::RenderInformation()
{
	ImGui::Begin("Debug Info");

	if (playerEntity != nullptr)
	{
		ImGui::Text("Position: "); ImGui::SameLine();
		ImGui::Text(std::to_string(playerEntity->GetComponent<Firelight::ECS::TransformComponent>()->GetPosition().x).c_str());  ImGui::SameLine();
		ImGui::Text(std::to_string(playerEntity->GetComponent<Firelight::ECS::TransformComponent>()->GetPosition().y).c_str());
		ImGui::Text("Velocity: "); ImGui::SameLine();
		ImGui::Text(std::to_string(playerEntity->GetComponent<Firelight::ECS::RigidBodyComponent>()->velocity.x).c_str());  ImGui::SameLine();
		ImGui::Text(std::to_string(playerEntity->GetComponent<Firelight::ECS::RigidBodyComponent>()->velocity.y).c_str());
	}
	ImGui::End();
}

void ImGuiPlayerLayer::RenderDebugCraftingMenu()
{
	if (m_craftingRecipes != nullptr)
	{
		ImGui::Begin("Crafting");

		int recipeIndex = 0;
		for (auto* recipe : *m_craftingRecipes)
		{
			if (auto* itemComponent = ItemDatabase::Instance()->GetItemComponentFromId(recipe->GetItemToMake()))
			{
				ImGui::Text(itemComponent->name.c_str()); ImGui::SameLine();
			}
			else
			{
				continue;
			}

			ImGui::Text(" [");

			auto& requiredItems = recipe->GetRequiredItems();
			int itemIndex = 0;
			for (auto& itemRequirement : requiredItems)
			{
				ImGui::SameLine();

				if (auto* requiredItemComponent = ItemDatabase::Instance()->GetItemComponentFromId(itemRequirement.m_itemId))
				{
					ImGui::Text((std::to_string(itemRequirement.m_numRequired) + " " + requiredItemComponent->name).c_str()); ImGui::SameLine();
				}

				if (itemIndex < requiredItems.size() - 1)
				{
					ImGui::Text(",");
				}

				++itemIndex;
			}

			ImGui::SameLine();

			ImGui::Text("]");

			ImGui::SameLine();

			ImGui::PushID(recipeIndex);

			if (ImGui::Button("Craft"))
			{
				recipe->Craft("PlayerInventory");
				Firelight::Events::EventDispatcher::InvokeFunctions<Firelight::Events::Inventory::UpdateCraftableItemsEvent>();
			}

			ImGui::PopID();

			++recipeIndex;
		}

		ImGui::End();
	}
}

void ImGuiPlayerLayer::SetDebugCraftingMenuEnabled(bool enabled)
{
	m_debugCraftingMenuEnabled = enabled;

	if (!enabled)
	{
		m_craftingRecipes = nullptr;
	}
}

void ImGuiPlayerLayer::GiveAvailableCraftingRecipes(const std::vector<const CraftingRecipe*>* craftingRecipes)
{
	m_craftingRecipes = craftingRecipes;
}

void ImGuiPlayerLayer::SetupTheme()
{
	auto& colors = ImGui::GetStyle().Colors;

	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.4f, 0.405f, 0.41f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.25f, 0.2505f, 0.251f, 1.0f };

	// Frame Background
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}
