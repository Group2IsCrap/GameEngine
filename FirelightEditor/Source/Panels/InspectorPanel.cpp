#include "InspectorPanel.h"

#include <Source/ECS/Entity.h>
#include <Source/ECS/Components/BasicComponents.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include <Source/ECS/Components/RenderingComponents.h>
#include <Source/ECS/EntityComponentSystem.h>
#include <Source/Graphics/AssetManager.h>

#include <filesystem>

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

template<typename T>
static void DrawEnum(const char* title, const char** items, size_t itemSize, T& valueToChange)
{
	ImGui::Text(title);
	ImGui::SameLine();
	int index = static_cast<int>(valueToChange);
	if (ImGui::BeginCombo(("##ComboBox" + std::string(title)).c_str(), items[index]))
	{
		for (unsigned int i = 0; i < itemSize; ++i)
		{
			bool is_selected = index == i;
			if (ImGui::Selectable(items[i], is_selected))
			{
				valueToChange = static_cast<T>(i);
				if (is_selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
		}
		ImGui::EndCombo();
	}
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

static void DrawVec2Control(const std::string& label, float& x, float& y, float columnWidth = 80.0f)
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

	ImGui::PopStyleVar();

	ImGui::PopID();

	ImGui::Columns(1);

	ImGui::Indent();
	ImGui::Spacing();
}

template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, Firelight::ECS::Entity* entity, UIFunction uiFunction, bool allowDeletion = false)
{
	if (!entity->HasComponent<T>())
	{
		return;
	}

	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

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
		ImGui::SetNextItemWidth(180.0f);
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			tag = std::string(buffer);

		ImGui::SameLine();
		ImGui::Text("Static");
		auto& isStatic = entity->GetComponent<Firelight::ECS::StaticComponent>()->isStatic;
		ImGui::SameLine();
		ImGui::Checkbox("##Static", &isStatic);

		ImGui::Spacing();

		DrawComponent<Firelight::ECS::TransformComponent>("Transform", entity, [](auto& component)
		{
			ImGui::Spacing();
			DrawVec3Control("Position", component->position.x, component->position.y, component->position.z);
			ImGui::Unindent();
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Rotation");
			ImGui::SameLine();
			ImGui::DragFloat("##Rotation", &component->rotation, 0.1f, 0.0f, 0.0f, "%.2f");
			//ImGui::InputFloat("##Rotation", &component->rotation, 0.1f, 0.0f, "%.2f");
			if (component->rotation > 360)
				component->rotation = 0;
			if (component->rotation < 0)
				component->rotation = 360;
			ImGui::Indent();
			ImGui::Spacing();
			DrawVec3Control("Scale", component->scale.x, component->scale.y, component->scale.z);
			ImGui::Spacing();
		});

		DrawComponent<Firelight::ECS::SpriteComponent>("Sprite Renderer", entity, [](auto& component)
		{
			ImGui::Spacing();
			ImGui::Unindent();
			//const char* items[] = { "NDC", "Pixel", "WorldSpace" };
			//DrawEnum<Firelight::ECS::SpriteComponent::DrawSpace>("Draw Space", &items[0], 3, component->drawSpace);

			ImGui::AlignTextToFramePadding();
			ImGui::Text("Layer");
			ImGui::SameLine();
			ImGui::SliderInt("##Layer", &component->layer, 0, 64);
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Pixels Per Unit");
			ImGui::SameLine();
			ImGui::InputFloat("##PixelsPerUnit", &component->pixelsPerUnit, 1.0f, 10.0f, "%0.0f");
			if (component->pixelsPerUnit < 1)
				component->pixelsPerUnit = 1;
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Sprite");
			ImGui::SameLine();
			if (component->texture == nullptr)
				component->texture = Firelight::Graphics::AssetManager::Instance().GetTexture("$ENGINE/Textures/missing.png");
			ImGui::ImageButton((ImTextureID)component->texture->GetShaderResourceView().Get(), { 50, 50 });

			// Drag and drop texture
			Firelight::Graphics::Texture* texture = component->texture;
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
						}
					}
				}
				ImGui::EndDragDropTarget();
			}

			component->texture = texture;

			ImGui::Indent();
			ImGui::Spacing();
		}, true);

		DrawComponent<Firelight::ECS::RigidBodyComponent>("RigidBody", entity, [](auto& component)
		{
			ImGui::Spacing();
			DrawVec3Control("Velocity", component->velocity.x, component->velocity.y, component->velocity.z);
			ImGui::Spacing();
		}, true);

		DrawComponent<Firelight::ECS::BoxColliderComponent>("Box Collider", entity, [](auto& component)
		{
			ImGui::Spacing();
			ImGui::Unindent();
			ImGui::Checkbox("Enabled", &component->isEnabled);
			ImGui::Indent();
			ImGui::Spacing();
		}, true);

		DrawComponent<Firelight::ECS::CircleColliderComponent>("Circle Collider", entity, [](auto& component)
		{
			ImGui::Spacing();
			ImGui::Unindent();
			ImGui::Checkbox("Enabled", &component->isEnabled);
			ImGui::Text("Size");
			ImGui::SameLine();
			ImGui::AlignTextToFramePadding();
			ImGui::DragFloat("##Size", &component->radius, 0.1f, 0.1f, 100.0f, "%.2f");
			ImGui::Indent();
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
			if (ImGui::MenuItem("Sprite Renderer"))
			{
				if (!m_selectionContext->HasComponent<Firelight::ECS::SpriteComponent>())
				{
					m_selectionContext->AddComponent<Firelight::ECS::SpriteComponent>(new Firelight::ECS::SpriteComponent());
				}
			}

			if (ImGui::MenuItem("RigidBody"))
			{
				if (!m_selectionContext->HasComponent<Firelight::ECS::RigidBodyComponent>())
					m_selectionContext->AddComponent<Firelight::ECS::RigidBodyComponent>(new Firelight::ECS::RigidBodyComponent());
				else ERROR_SILENT("Entity already has a RigidBody Component!");
			}
			if (ImGui::MenuItem("Box Collider"))
			{
				m_selectionContext->AddComponent<Firelight::ECS::BoxColliderComponent>(new Firelight::ECS::BoxColliderComponent());
			}
			if (ImGui::MenuItem("Circle Collider"))
			{
				m_selectionContext->AddComponent<Firelight::ECS::CircleColliderComponent>(new Firelight::ECS::CircleColliderComponent());
			}
			ImGui::EndPopup();
		}
	}
}