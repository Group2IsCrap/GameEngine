#pragma once

#include "Source/ImGuiUI/ImGuiLayer.h"
#include "Source/ECS/Entity.h"

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

	void RenderHiearchy();
	void DrawEntityNode(Firelight::ECS::Entity* gameEntity);

private:
	std::vector<Firelight::ECS::Entity*> m_entitiesInScene;
	Firelight::ECS::Entity* m_selectionContext;

};