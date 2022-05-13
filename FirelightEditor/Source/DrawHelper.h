#pragma once

#include <Source/ECS/Components/BasicComponents.h>
#include <Source/ECS/Components/PhysicsComponents.h>
#include <Source/ECS/Components/RenderingComponents.h>
#include <Source/ECS/Components/AnimationComponent.h>
#include "Source/ECS/EntityWrappers/Entity.h"

#include "Includes/imgui/imgui.h"
#include "Includes/imgui/imgui_internal.h"

#include <vector>

namespace Firelight::Editor
{
	class DrawHelper
	{
	public:

		template<typename T>
		static void DrawComponent(const std::string& name, Firelight::ECS::Entity* entity, bool allowDeletion = false)
		{
			if (!entity->HasComponent<T>())
			{
				return;
			}

			auto components = entity->GetComponents<T>();
			for (unsigned int i = 0; i < components.size(); ++i)
			{
				auto c = components[i];

				const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

				ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
				float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
				ImGui::Separator();
				bool open = ImGui::TreeNodeEx((void*)c, treeNodeFlags, name.c_str());
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
						entity->RemoveComponent<T>(i);
					}
				}

				if (open)
				{
					DrawHelper::DrawComponentType(c);
					sm_componentCount++;
					ImGui::TreePop();
				}
			}
		}

		static void DrawVec3Control(const std::string& label, float& x, float& y, float& z, float columnWidth = 80.0f);

		static void DrawVec2Control(const std::string& label, float& x, float& y, float columnWidth = 80.0f);

		static void DrawEnumControl(const std::string& label, int* selectedItem, std::vector<std::string> values);

		static void DrawImage(const std::string& label, Firelight::ECS::BaseComponent* component, const std::string& texturePath);

		static void DrawComponentType(Firelight::ECS::BaseComponent* component);
		static void DrawComponentType(Firelight::ECS::TransformComponent* component);
		static void DrawComponentType(Firelight::ECS::SpriteComponent* component);
		static void DrawComponentType(Firelight::ECS::AnimationComponent* component);
		static void DrawComponentType(Firelight::ECS::RigidBodyComponent* component);
		static void DrawComponentType(Firelight::ECS::BoxColliderComponent* component);
		static void DrawComponentType(Firelight::ECS::CircleColliderComponent* component);

		static void ResetUniqueID();

	private:
		static std::string GetUniqueID(std::string name, bool hasName = false);

	private:
		static int sm_componentCount;
	};
}
