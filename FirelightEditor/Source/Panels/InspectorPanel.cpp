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
	ImGui::Begin("Inspector");

	if (m_selectionContext)
		DrawComponents(m_selectionContext);

	ImGui::End();
}

static void DrawVec3Control(const std::string& label, float& x, float& y, float& z, float columnWidth = 80.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Unindent();
	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::AlignTextToFramePadding();
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
	ImGui::AlignTextToFramePadding();
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##X", &x, 0.1f, 0.0f, 0.0f, "%.2f");
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
	ImGui::DragFloat("##Y", &y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	ImGui::Button("Z", buttonSize);
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::PopID();

	ImGui::Columns(1);

	ImGui::Indent();
	ImGui::Spacing();
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
		ImGui::PopStyleVar();


		if (allowDeletion)
		{
			ImGui::SameLine((2.0f + contentRegionAvailable.x - lineHeight * 0.5f) - 5.0f);
			if (ImGui::Button("...", ImVec2{ lineHeight + 5.0f, lineHeight }))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;

				ImGui::EndPopup();
			}

			if (removeComponent)
			{
				entity->RemoveComponent<T>();
			}
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
		ImGui::Spacing();

		DrawComponent<Firelight::ECS::TransformComponent>("Transform", entity, [](auto& component)
		{
			ImGui::Spacing();
			DrawVec3Control("Position", component->position.x, component->position.y, component->position.z);
			DrawVec3Control("Scale", component->scale.x, component->scale.y, component->scale.z);
			ImGui::Spacing();
		});

		DrawComponent<Firelight::ECS::PhysicsComponent>("Physics", entity, [](auto& component)
		{
			ImGui::Spacing();
			DrawVec3Control("Velocity", component->velocity.x, component->velocity.y, component->velocity.z);
			ImGui::Spacing();
		}, true);

		ImGui::Spacing();
		ImGui::PushItemWidth(-1);
		ImVec2 buttonSize = { ImGui::GetWindowWidth() - 15.0f, 25.0f };
		if (ImGui::Button("Add Component", buttonSize))
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
