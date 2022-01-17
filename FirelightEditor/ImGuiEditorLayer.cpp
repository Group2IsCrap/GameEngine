#include "ImGuiEditorLayer.h"

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"

#include <Source/ECS/Entity.h>
#include <Source/ECS/GameEntity.h>
#include <Source/ECS/Components.h>

using namespace Firelight::ECS;

ImGuiEditorLayer::ImGuiEditorLayer()
{
	m_selectionContext = {};

	GameEntity* test = new GameEntity();
	test->GetComponent<IdentificationComponent>()->name = "TestObject";

	m_entitiesInScene.push_back(test);
}

ImGuiEditorLayer::~ImGuiEditorLayer()
{
	for (auto entity : m_entitiesInScene)
	{
		delete entity;
		entity = nullptr;
	}
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
	RenderHiearchy();
	

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

void ImGuiEditorLayer::RenderHiearchy()
{
	// ImGui Stuff
	ImGui::Begin("Scene Hierarchy");

	for (auto item : m_entitiesInScene)
	{
		DrawEntityNode(item);
	}

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		m_selectionContext = {};

	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		ImGui::MenuItem("New Game Entity");
		/*
		if (ImGui::MenuItem("Create Empty Entity"))
			// CREATE NEW ENTITY HERE
		*/

		ImGui::EndPopup();
	}

	ImGui::End();
}

void ImGuiEditorLayer::DrawEntityNode(Entity* entity)
{
	auto tag = entity->GetComponent<IdentificationComponent>()->name;

	// Don't show arrow if the object does not have any child objects
	ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

	// Selected node
	if (ImGui::IsItemClicked())
		m_selectionContext = entity;

	bool isEntityDeleted = false;
	// Right-click to delete
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			isEntityDeleted = true;
		ImGui::EndPopup();
	}

	if (opened)
	{
		// Draw Child Nodes Here
		ImGui::TreePop();
	}

	if (isEntityDeleted)
	{
		// Destroy entity here
		if (m_selectionContext == entity)
			m_selectionContext = {};
	}
}
