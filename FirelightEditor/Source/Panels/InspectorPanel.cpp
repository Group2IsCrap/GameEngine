#include "InspectorPanel.h"

#include <Source/ECS/Entity.h>
#include <Source/ECS/Components.h>
#include <Source/ECS/EntityComponentSystem.h>

InspectorPanel::InspectorPanel()
{
	m_selectionContext = {};
}

void InspectorPanel::Draw()
{
	ImGui::Begin("Properties");

	if (m_selectionContext)
		DrawComponents(m_selectionContext);

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
static void DrawComponent(const std::string& name, Firelight::ECS::Entity* entity, UIFunction uiFunction, bool allowDeletion = false)
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

void InspectorPanel::DrawComponents(Firelight::ECS::Entity* entity)
{
	if (entity->HasComponent<Firelight::ECS::IdentificationComponent>())
	{
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Name:");
		ImGui::SameLine();
		auto& tag = entity->GetComponent<Firelight::ECS::IdentificationComponent>()->name;

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, tag.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			tag = std::string(buffer);

		DrawComponent<Firelight::ECS::TransformComponent>("Transform", entity, [](auto& component)
			{
				DrawVec2Control("Position", component->posX, component->posY);
			});

		DrawComponent<Firelight::ECS::PhysicsComponent>("Physics", entity, [](auto& component)
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
				if (!m_selectionContext->HasComponent<Firelight::ECS::PhysicsComponent>())
					m_selectionContext->AddComponent<Firelight::ECS::PhysicsComponent>(new Firelight::ECS::PhysicsComponent());
				else ERROR_SILENT("Entity already has a Physics Component!");
			}
			ImGui::EndPopup();
		}
	}
}
