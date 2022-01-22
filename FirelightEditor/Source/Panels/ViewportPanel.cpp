#include "ViewportPanel.h"

#include "Source/Graphics/GraphicsHandler.h"

ViewportPanel::ViewportPanel()
{
}

ViewportPanel::~ViewportPanel()
{
}

void ViewportPanel::Draw()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	ImGui::Begin("Viewport");

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImGui::Image((void*)Firelight::Graphics::GraphicsHandler::Instance().GetFinalImage().m_shaderResourceView.Get(), { windowSize.x, windowSize.y });

	ImGui::End();
	ImGui::PopStyleVar();
}
