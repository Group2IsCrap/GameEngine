#include "HierarchyPanel.h"

#include <Source/ECS/EntityComponentSystem.h>
#include <Source/ECS/Entity.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/ECS/GameEntity.h>


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
		if (ImGui::MenuItem("Game Entity"))
		{
			NewGameEntity();
		}

		if (ImGui::MenuItem("Camera"))
		{
			NewCameraEntity();
		}

		if (ImGui::MenuItem("Sprite"))
		{
			NewSpriteEntity();
		}
		

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
		m_selectionContext = {};
		m_inspectorPanel->m_selectionContext = {};
		DeleteGameEntity(entity);
	}
}

void HierarchyPanel::NewGameEntity()
{
	Firelight::ECS::GameEntity* newEntity = new Firelight::ECS::GameEntity();
	newEntity->GetComponent<Firelight::ECS::IdentificationComponent>()->name = "Game Entity";
	m_entitiesInScene.push_back(newEntity);
}

void HierarchyPanel::NewCameraEntity()
{
	Firelight::ECS::GameEntity* newEntity = new Firelight::ECS::GameEntity();
	newEntity->GetComponent<Firelight::ECS::IdentificationComponent>()->name = "Camera";
	m_entitiesInScene.push_back(newEntity);
}

void HierarchyPanel::NewSpriteEntity()
{
	Firelight::ECS::GameEntity* newEntity = new Firelight::ECS::GameEntity();
	newEntity->GetComponent<Firelight::ECS::IdentificationComponent>()->name = "Game Entity";
	m_entitiesInScene.push_back(newEntity);
}

void HierarchyPanel::DeleteGameEntity(Firelight::ECS::Entity* gameEntity)
{
	m_entitiesInScene.erase(std::remove(m_entitiesInScene.begin(), m_entitiesInScene.end(), gameEntity), m_entitiesInScene.end());
	
	delete gameEntity;
	gameEntity = nullptr;
}

