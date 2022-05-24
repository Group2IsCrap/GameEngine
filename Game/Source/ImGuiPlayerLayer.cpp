#include "ImGuiPlayerLayer.h"

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"

#include "Source/ECS/EntityWrappers/GameEntity.h"
#include "Source/ECS/Components/BasicComponents.h"
#include "Source/ECS/Components/PhysicsComponents.h"
#include "PCG/BiomeGeneration.h"

ImGuiPlayerLayer::ImGuiPlayerLayer()
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
		ImGui::Text("Is In Void: "); ImGui::SameLine();
		ImGui::Text(std::to_string(BiomeGeneration::Instance()->IsInVoid(BiomeGeneration::Instance()->testPosition)).c_str());
	}
	ImGui::End();
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
