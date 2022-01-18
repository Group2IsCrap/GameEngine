#include "ImGuiEditorLayer.h"

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"

#include <Source/ECS/Entity.h>
#include <Source/ECS/GameEntity.h>
#include <Source/ECS/Components.h>
#include <Source/Utils/ErrorManager.h>
#include <Source/Maths/Vec2.h>
#include <Source/Graphics/AssetManager.h>
#include <Source/Utils/StringHelpers.h>

using namespace Firelight::ECS;
using namespace Firelight::Maths;
using namespace Firelight::Graphics;
using namespace Firelight::Utils;

static const std::filesystem::path s_assetPath = "Assets";

#define _CRT_SECURE_NO_WARNINGS

ImGuiEditorLayer::ImGuiEditorLayer() : m_CurrentDirectory(s_assetPath)
{
	m_selectionContextHierarchy = {};

	GameEntity* test = new GameEntity();
	test->GetComponent<IdentificationComponent>()->name = "TestObject";

	m_entitiesInScene.push_back(test);

	// TODO : Create textures for icons
	m_DirectoryIcon = AssetManager::Instance().GetTexture("Icons/DirectoryIcon.png");
	m_FileIcon = AssetManager::Instance().GetTexture("Icons/FileIcon.png");
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
	RenderHiearchy();
	RenderPropertiesPanel();
	RenderContentBrowserPanel();
	

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
		m_selectionContextHierarchy = {};

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
	ImGuiTreeNodeFlags flags = ((m_selectionContextHierarchy == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

	// Selected node
	if (ImGui::IsItemClicked())
		m_selectionContextHierarchy = entity;

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
		if (m_selectionContextHierarchy == entity)
			m_selectionContextHierarchy = {};
	}
}

void ImGuiEditorLayer::RenderPropertiesPanel()
{
	ImGui::Begin("Properties");

	if (m_selectionContextHierarchy)
		DrawComponents(m_selectionContextHierarchy);

	ImGui::End();
}

static void DrawVec2Control(const std::string& label, int& x, int& y, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);
	ImGui::Button("X", buttonSize);
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragInt("##X", &x, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	ImGui::Button("Y", buttonSize);
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragInt("##Y", &y, 1.0f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Entity* entity, UIFunction uiFunction, bool allowDeletion = false)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity->HasComponent<T>())
	{
		auto component = entity->GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar(
		);

		if (allowDeletion)
		{
			ImGui::Spacing();
			if (ImGui::Button("Remove Component"))
				entity->RemoveComponent<T>();
			ImGui::Spacing();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}
	}
}

void ImGuiEditorLayer::DrawComponents(Firelight::ECS::Entity* gameEntity)
{
	if (gameEntity->HasComponent<IdentificationComponent>())
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name:");
		ImGui::SameLine();
		auto& tag = gameEntity->GetComponent<IdentificationComponent>()->name;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			tag = std::string(buffer);

		DrawComponent<TransformComponent>("Transform", gameEntity, [](auto& component)
		{
			DrawVec2Control("Position", component->posX, component->posY);
		});

		DrawComponent<PhysicsComponent>("Physics", gameEntity, [](auto& component)
		{
			DrawVec2Control("Velocity", component->velX, component->velY);
		}, true);

		ImGui::Spacing();
		ImGui::PushItemWidth(-1);
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("AddComponent");
		}
		ImGui::PopItemWidth();

		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Physics"))
			{
				if (!m_selectionContextHierarchy->HasComponent<PhysicsComponent>())
					m_selectionContextHierarchy->AddComponent<PhysicsComponent>(new PhysicsComponent());
				else ERROR_SILENT("Entity already has a Physics Component!");
			}
			ImGui::EndPopup();
		}
	}
}

void ImGuiEditorLayer::RenderContentBrowserPanel()
{
	ImGui::Begin("Content Browser");

	
	ImGui::BeginDisabled(m_CurrentDirectory == std::filesystem::path(s_assetPath));
	if (ImGui::Button("<-"))
	{
		m_CurrentDirectory = m_CurrentDirectory.parent_path();
	}
	ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::Text(m_CurrentDirectory.relative_path().string().c_str());

	static float padding = 16.0f;
	static float thumbnailSize = 64.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directoryEntry.path();

		std::string lowerPath = path.string();
		StringHelpers::StringToLower(lowerPath);
		if (lowerPath.starts_with("assets\\$"))
			continue;

		auto relativePath = std::filesystem::relative(path, s_assetPath);
		std::string filenameString = relativePath.filename().string();

		ImGui::PushID(filenameString.c_str());
		// Set icon based on whether it is a file or not
		
		Texture* icon = directoryEntry.is_directory() ? m_DirectoryIcon : m_FileIcon;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)icon->GetShaderResourceView().Get(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
		//ImGui::Button("ICON HERE");

		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::EndDragDropSource();
		}

		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				m_CurrentDirectory /= path.filename();

		}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::Columns(1);

	ImGui::End();
}
