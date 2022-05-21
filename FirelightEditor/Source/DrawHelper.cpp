#include "DrawHelper.h"

#include "Source/Graphics/AssetManager.h"
#include "Source/Utils/ErrorManager.h"

namespace Firelight::Editor
{
	int DrawHelper::sm_componentCount = 0;

	void DrawHelper::DrawVec3Control(const std::string& label, float& x, float& y, float& z, float columnWidth)
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
		ImGui::Button(GetUniqueID("X", true).c_str(), buttonSize);
		ImGui::AlignTextToFramePadding();
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat(GetUniqueID("X").c_str(), &x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		ImGui::Button(GetUniqueID("Y", true).c_str(), buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat(GetUniqueID("Y").c_str(), &y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushFont(boldFont);
		ImGui::Button(GetUniqueID("Z", true).c_str(), buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat(GetUniqueID("Z").c_str(), &z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::PopID();

		ImGui::Columns(1);

		ImGui::Indent();
		ImGui::Spacing();
	}
	
	void DrawHelper::DrawVec2Control(const std::string& label, float& x, float& y, float columnWidth)
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
		ImGui::Button(GetUniqueID("X", true).c_str(), buttonSize);
		ImGui::AlignTextToFramePadding();
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat(GetUniqueID("X").c_str(), &x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
		ImGui::PushFont(boldFont);
		ImGui::Button(GetUniqueID("Y", true).c_str(), buttonSize);
		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat(GetUniqueID("Y").c_str(), &y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::PopID();

		ImGui::Columns(1);

		ImGui::Indent();
		ImGui::Spacing();
	}

	void DrawHelper::DrawEnumControl(const std::string& label, int* selectedItem, std::vector<std::string> values)
	{
		if (values.size() <= 0)
		{
			return;
		}

		ASSERT_FATAL(selectedItem > (int*)values.size(), "Selected Array Index Larger Than Array");

		ImGui::Text(label.c_str());
		ImGui::SameLine();
		std::string id = "##EnumCombo";
		if (ImGui::BeginCombo((id + label).c_str(), values[*selectedItem].c_str()))
		{
			for (int i = 0; i < values.size(); i++)
			{
				bool isSelected = &selectedItem == nullptr ? false : (values[*selectedItem].c_str() == values[i]);
				if (ImGui::Selectable(values[i].c_str(), isSelected))
				{
					*selectedItem = i;
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
			}

			ImGui::EndCombo();
		}
	}

	void DrawHelper::DrawImage(const std::string& label, Firelight::ECS::BaseComponent* component, const std::string& texturePath)
	{
		Firelight::ECS::SpriteComponent* spriteComponent = dynamic_cast<Firelight::ECS::SpriteComponent*>(component);
		if (component == nullptr)
		{
			return;
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text(label.c_str());
		ImGui::SameLine();
		if (spriteComponent->texture == nullptr)
			spriteComponent->texture = Firelight::Graphics::AssetManager::Instance().GetTexture(texturePath);
		ImGui::ImageButton((ImTextureID)spriteComponent->texture->GetShaderResourceView().Get(), { 50, 50 });

		// Drag and drop texture
		Firelight::Graphics::Texture* texture = spriteComponent->texture;
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

		spriteComponent->texture = texture;
	}
	
	void DrawHelper::DrawComponentType(Firelight::ECS::BaseComponent* component)
	{
	}

	void DrawHelper::DrawComponentType(Firelight::ECS::TransformComponent* component)
	{
		ImGui::Spacing();
		//DrawVec3Control("Position", component->GetPosition().x, component->GetPosition().y, component->GetPosition().z);
		ImGui::Unindent();
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat(GetUniqueID("Rotation").c_str(), &component->rotation, 0.1f, 0.0f, 0.0f, "%.2f");
		if (component->rotation > 360)
			component->rotation = 0;
		if (component->rotation < 0)
			component->rotation = 360;
		ImGui::Indent();
		ImGui::Spacing();
		DrawVec3Control("Scale", component->scale.x, component->scale.y, component->scale.z);
		ImGui::Spacing();
	}
	
	void DrawHelper::DrawComponentType(Firelight::ECS::SpriteComponent* component)
	{
		ImGui::Spacing();
		ImGui::Unindent();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Layer");
		ImGui::SameLine();
		ImGui::SliderInt(GetUniqueID("Layer").c_str(), &component->layer, 0, 64);
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Pixels Per Unit");
		ImGui::SameLine();
		ImGui::InputFloat(GetUniqueID("PixelsPerUnit").c_str(), &component->pixelsPerUnit, 1.0f, 10.0f, "%0.0f");
		if (component->pixelsPerUnit < 1)
			component->pixelsPerUnit = 1;
		
		DrawImage("Sprite", component, "$ENGINE/Textures/missing.png");

		ImGui::Indent();
		ImGui::Spacing();
	}
	
	void DrawHelper::DrawComponentType(Firelight::ECS::AnimationComponent* component)
	{
		ImGui::Spacing();
		ImGui::Unindent();
		ImGui::Text("Test");
		ImGui::Indent();
		ImGui::Spacing();
	}

	void DrawHelper::DrawComponentType(Firelight::ECS::RigidBodyComponent* component)
	{
		ImGui::Spacing();
		DrawVec3Control("Velocity", component->velocity.x, component->velocity.y, component->velocity.z);
		ImGui::Spacing();
	}
	
	void DrawHelper::DrawComponentType(Firelight::ECS::BoxColliderComponent* component)
	{
		ImGui::Spacing();
		ImGui::Unindent();
		ImGui::Checkbox(GetUniqueID("Enabled", true).c_str(), &component->isEnabled);
		ImGui::Indent();
		ImGui::Spacing();
	}
	
	void DrawHelper::DrawComponentType(Firelight::ECS::CircleColliderComponent* component)
	{
		ImGui::Spacing();
		ImGui::Unindent();
		ImGui::Checkbox(GetUniqueID("Enabled", true).c_str(), &component->isEnabled);
		ImGui::Text("Size");
		ImGui::SameLine();
		ImGui::AlignTextToFramePadding();
		ImGui::DragFloat(GetUniqueID("Size").c_str(), &component->radius, 0.1f, 0.1f, 100.0f, "%.2f");
		ImGui::Indent();
		ImGui::Spacing();
	}

	void DrawHelper::ResetUniqueID()
	{
		sm_componentCount = 0;
	}
	
	std::string DrawHelper::GetUniqueID(std::string name, bool hasName)
	{
		std::string uniqueName = ((hasName ? name : "") + "##" + name + std::to_string(sm_componentCount));
		return uniqueName;
	}
}