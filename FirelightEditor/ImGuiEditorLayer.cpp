#include "ImGuiEditorLayer.h"

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"

#include "Source/ECS/EntityWrappers/GameEntity.h"

ImGuiEditorLayer::ImGuiEditorLayer()
{
	m_contentBrowserPanel = new ContentBrowserPanel();
	m_hierarchyPanel = new HierarchyPanel();
	m_inspectorPanel = new InspectorPanel();
	m_viewportPanel = new ViewportPanel();
	m_animationWindow = new AnimationWindow();

	m_selectionContextHierarchy = {};

	m_hierarchyPanel->m_inspectorPanel = m_inspectorPanel;

	m_drawAnimationWindow = false;

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("Assets/$editor/fonts/opensans/OpenSans-Bold.ttf", 16.0f);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/$editor/fonts/opensans/OpenSans-Regular.ttf", 16.0f);

	SetupTheme();
}

ImGuiEditorLayer::~ImGuiEditorLayer()
{
	for (auto entity : m_hierarchyPanel->m_entitiesInScene)
	{
		delete entity;
		entity = nullptr;
	}

	m_hierarchyPanel->m_entitiesInScene.clear();

	delete m_contentBrowserPanel;
	m_contentBrowserPanel = nullptr;

	delete m_hierarchyPanel;
	m_hierarchyPanel = nullptr;

	delete m_inspectorPanel;
	m_inspectorPanel = nullptr;

	delete m_viewportPanel;
	m_viewportPanel = nullptr;

	delete m_animationWindow;
	m_animationWindow = nullptr;
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

	ImGuiStyle& style = ImGui::GetStyle();
	float minWinSizeX = style.WindowMinSize.x;
	style.WindowMinSize.x = 325.0f;

	ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDockSpace");
	ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::DockSpaceOverViewport();

	style.WindowMinSize.x = minWinSizeX;
	
	RenderMenuBar();

	m_contentBrowserPanel->Draw();
	m_hierarchyPanel->Draw();
	m_inspectorPanel->Draw();
	m_viewportPanel->Draw();

	if (m_animationWindow->m_isOpen)
		m_animationWindow->Draw();

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
			{
				m_hierarchyPanel->NewGameEntity();
			}

			ImGui::MenuItem("Reset Camera Position");
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Animation Window"))
			{
				m_animationWindow->m_isOpen = true;
			}
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

void ImGuiEditorLayer::SetupTheme()
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
