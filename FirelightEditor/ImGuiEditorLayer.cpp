#include "ImGuiEditorLayer.h"

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"

#include <Source/ECS/GameEntity.h>

ImGuiEditorLayer::ImGuiEditorLayer()
{
	m_contentBrowserPanel = new ContentBrowserPanel();
	m_hierarchyPanel = new HierarchyPanel();
	m_inspectorPanel = new InspectorPanel();

	m_selectionContextHierarchy = {};

	Firelight::ECS::GameEntity* test = new Firelight::ECS::GameEntity();
	test->GetComponent<Firelight::ECS::IdentificationComponent>()->name = "TestObject";

	m_hierarchyPanel->m_entitiesInScene.push_back(test);
	m_hierarchyPanel->m_inspectorPanel = m_inspectorPanel;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/$editor/fonts/opensans/OpenSans-Regular.ttf", 16.0f);
}

ImGuiEditorLayer::~ImGuiEditorLayer()
{
	for (auto entity : m_entitiesInScene)
	{
		delete entity;
		entity = nullptr;
	}

	m_entitiesInScene.clear();
}

void ImGuiEditorLayer::Render()
{
	// Create the docking environment
	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
		ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);
	


	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("InvisibleWindow", nullptr, windowFlags);
	ImGui::PopStyleVar(3);


	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::DockSpaceOverViewport();
	
	RenderMenuBar();

	m_contentBrowserPanel->Draw();
	m_hierarchyPanel->Draw();
	m_inspectorPanel->Draw();

	ImGui::End();
}

void ImGuiEditorLayer::RenderMenuBar()
{
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("New Project");
			ImGui::MenuItem("Open Project");
			ImGui::MenuItem("New Scene");
			ImGui::MenuItem("Open Scene");
			ImGui::MenuItem("Save");
			ImGui::MenuItem("Save As");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("New Game Entity"))

			ImGui::MenuItem("Reset Camera Position");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			ImGui::MenuItem("View Documentation");
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	ImGui::PopStyleVar(1);
}