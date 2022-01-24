#include "HierarchyPanel.h"

#include "Source/ECS/EntityComponentSystem.h"
#include "Source/ECS/EntityWrappers/Entity.h"
#include "Source/ECS/Components/BasicComponents.h"
#include "Source/ECS/EntityWrappers/GameEntity.h"
#include "Source/ECS/EntityWrappers/SpriteEntity.h"
#include "Source/ECS/EntityWrappers/CameraEntity.h"
#include "Source/Graphics/AssetManager.h"


#include "InspectorPanel.h"

HierarchyPanel::HierarchyPanel()
{
	m_selectionContext = {};
}

void HierarchyPanel::Draw()
{
	ImGui::Begin("Scene Hierarchy");

	// Right-click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, true))
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

	ImVec2 windowSize = ImGui::GetContentRegionAvail();
	ImGui::SetItemAllowOverlap();
	ImGui::InvisibleButton("##InvisibleButton", windowSize);
	ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
	ImGui::SetItemAllowOverlap();

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
	{
		m_selectionContext = {};
		m_inspectorPanel->m_selectionContext = {};
	}

	

	// Drag and drop texture
	Firelight::Graphics::Texture* texture = nullptr;
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM");
		if (payload != nullptr)
		{
			const wchar_t* path = (const wchar_t*)payload->Data;
			if (path != nullptr)
			{
				std::string pathString = Firelight::Utils::StringHelpers::WideStringToString(path);
				std::string pngExtention = pathString.substr(pathString.length() - 4);
				if (pngExtention == ".png")
				{
					texture = Firelight::Graphics::AssetManager::Instance().GetTexture(pathString);
					Firelight::ECS::SpriteEntity* entity = NewSpriteEntity();
					entity->GetComponent<Firelight::ECS::SpriteComponent>()->texture = texture;
					m_selectionContext = entity;
					m_inspectorPanel->m_selectionContext = entity;
				}
			}
		}
		ImGui::EndDragDropTarget();
	}

	for (auto item : m_entitiesInScene)
	{
		DrawEntityNode(item);
	}

	ImGui::End();
}

void HierarchyPanel::DrawEntityNode(Firelight::ECS::Entity* entity)
{
	auto tag = entity->GetComponent<Firelight::ECS::IdentificationComponent>()->name;

	// Don't show arrow if the object does not have any child objects
	ImGuiTreeNodeFlags flags = ((m_selectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx(entity, flags, tag.c_str());

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
	Firelight::ECS::CameraEntity* newEntity = new Firelight::ECS::CameraEntity();
	newEntity->GetComponent<Firelight::ECS::IdentificationComponent>()->name = "Camera";
	m_entitiesInScene.push_back(newEntity);
}

Firelight::ECS::SpriteEntity* HierarchyPanel::NewSpriteEntity()
{
	Firelight::ECS::SpriteEntity* newEntity = new Firelight::ECS::SpriteEntity();
	newEntity->GetComponent<Firelight::ECS::IdentificationComponent>()->name = "Game Entity";
	m_entitiesInScene.push_back(newEntity);
	return newEntity;
}

void HierarchyPanel::DeleteGameEntity(Firelight::ECS::Entity* gameEntity)
{
	m_entitiesInScene.erase(std::remove(m_entitiesInScene.begin(), m_entitiesInScene.end(), gameEntity), m_entitiesInScene.end());
	Firelight::ECS::EntityComponentSystem::Instance()->RemoveEntity(gameEntity->GetEntityID());
	delete gameEntity;
	gameEntity = nullptr;
}

