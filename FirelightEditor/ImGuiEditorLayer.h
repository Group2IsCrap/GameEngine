#pragma once

#include <filesystem>
#include "Source/ImGuiUI/ImGuiLayer.h"
#include "Source/ECS/Entity.h"
#include "Source/Graphics/Data/Texture.h"

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

	void RenderPropertiesPanel();
	void DrawComponents(Firelight::ECS::Entity* gameEntity);

	void RenderContentBrowserPanel();

private:
	std::vector<Firelight::ECS::Entity*> m_entitiesInScene;
	Firelight::ECS::Entity* m_selectionContextHierarchy;

	std::filesystem::path m_CurrentDirectory;

	Firelight::Graphics::Texture* m_DirectoryIcon;
	Firelight::Graphics::Texture* m_FileIcon;
};