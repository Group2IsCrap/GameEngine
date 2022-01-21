#include "AnimationWindow.h"

AnimationWindow::AnimationWindow()
{
}

AnimationWindow::~AnimationWindow()
{
}

void AnimationWindow::Draw()
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;
	ImVec2 animWindowSize = ImVec2(300.0f, 150.0f);
	ImGui::SetNextWindowSize(animWindowSize, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(
		(windowSize.x * 0.5f) - (animWindowSize.x * 0.5f), 
		(windowSize.y * 0.5f) - (animWindowSize.y * 0.5f)), ImGuiCond_FirstUseEver);

	ImGui::Begin("Animation Window");

	ImGui::End();
}
