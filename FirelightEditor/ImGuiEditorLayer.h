#pragma once

#include "Source/ImGuiUI/ImGuiLayer.h"
#include "Source/ECS/EntityWrappers/Entity.h"
#include "Source/Graphics/Data/Texture.h"
#include "Source/Panels/ContentBrowserPanel.h"
#include "Source/Panels/HierarchyPanel.h"
#include "Source/Panels/InspectorPanel.h"
#include "Source/Panels/ViewportPanel.h"
#include "Source/Panels/AnimationWindow.h"

#include <vector>

class ImGuiEditorLayer : public Firelight::ImGuiUI::ImGuiLayer
{
public:
	void Render() override;

public:

	ImGuiEditorLayer();
	~ImGuiEditorLayer();

private:

	void RenderMenuBar();
	void SetupTheme();

private:
	std::vector<Firelight::ECS::Entity*> m_entitiesInScene;
	Firelight::ECS::Entity* m_selectionContextHierarchy;

	ContentBrowserPanel* m_contentBrowserPanel;
	HierarchyPanel* m_hierarchyPanel;
	InspectorPanel* m_inspectorPanel;
	ViewportPanel* m_viewportPanel;
	AnimationWindow* m_animationWindow;

	bool m_drawAnimationWindow;
};