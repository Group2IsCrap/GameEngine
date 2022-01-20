#include "HierarchyPanel.h"

#include <Source/ECS/EntityComponentSystem.h>
#include <Source/ECS/Entity.h>
#include <Source/ECS/Components/BasicComponents.h>

#include "InspectorPanel.h"

HierarchyPanel::HierarchyPanel()
{
	m_selectionContext = {};
}

void HierarchyPanel::Draw()
{
	ImGui::Begin("Scene Hierarchy");

	for (auto item : m_entitiesInScene)
	{
		DrawEntityNode(item);
	}

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_selectionContext = {};
		m_inspectorPanel->m_selectionContext = {};
	}

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

void HierarchyPanel::DrawEntityNode(Firelight::ECS::Entity* entity)
{
	auto tag = entity->GetComponent<Firelight::ECS::IdentificationComponent>()->name;

	// Don't show arrow if the object does not have any child objects
	ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

	// Selected node
	if (ImGui::IsItemClicked())
	{
		m_selectionContext = entity;
		m_inspectorPanel->m_selectionContext = entity;
	}

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
		{
			m_selectionContext = {};
			m_inspectorPanel->m_selectionContext = {};
		}
	}
}
