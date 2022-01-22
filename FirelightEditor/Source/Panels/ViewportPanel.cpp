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
	ImGui::Begin("Viewport");

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImGui::Image((void*)Firelight::Graphics::GraphicsHandler::Instance().GetFinalImage().m_shaderResourceView.Get(), { windowSize.x, windowSize.y });

	ImGui::End();
}
