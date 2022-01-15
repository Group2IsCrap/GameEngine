#include "ImGuiTestLayer.h"

#include "Includes/imgui/imgui.h"

void ImGuiTestLayer::Render()
{
	ImGui::Begin("Test Window");
	ImGui::Text("Test");
	ImGui::End();
}
