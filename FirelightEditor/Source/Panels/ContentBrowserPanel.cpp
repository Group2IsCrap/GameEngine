#include "ContentBrowserPanel.h"

#include "Source/Graphics/AssetManager.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include <filesystem>

static const std::filesystem::path s_assetPath = "Assets";

#define _CRT_SECURE_NO_WARNINGS

static std::string s_folderName;

ContentBrowserPanel::ContentBrowserPanel() : m_currentDirectory(s_assetPath)
{
	m_directoryIcon = Firelight::Graphics::AssetManager::Instance().GetTexture("Icons/DirectoryIcon.png");
	m_fileIcon = Firelight::Graphics::AssetManager::Instance().GetTexture("Icons/FileIcon.png");
}

void ContentBrowserPanel::Draw()
{
	ImGui::Begin("Content Browser");

	// Right-click on blank space
	bool createFolder = false;
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Folder"))
				createFolder = true;

			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}

	if (createFolder)
	{
		ImGui::OpenPopup("New Folder");
	}

	CreateFolder();

	ImGui::BeginDisabled(m_currentDirectory == std::filesystem::path(s_assetPath));
	if (ImGui::Button("<--"))
	{
		m_currentDirectory = m_currentDirectory.parent_path();
	}
	ImGui::EndDisabled();
	ImGui::SameLine();
	ImGui::Text(m_currentDirectory.relative_path().string().c_str());

	static float padding = 16.0f;
	static float thumbnailSize = 64.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
	{
		const auto& path = directoryEntry.path();

		std::string lowerPath = path.string();
		Firelight::Utils::StringHelpers::StringToLower(lowerPath);
		if (lowerPath.starts_with("assets\\$"))
			continue;

		auto relativePath = std::filesystem::relative(path, s_assetPath);
		std::string filenameString = relativePath.filename().string();

		ImGui::PushID(filenameString.c_str());

		Firelight::Graphics::Texture* icon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::ImageButton((ImTextureID)icon->GetShaderResourceView().Get(), { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });

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
				m_currentDirectory /= path.filename();

		}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		ImGui::PopID();
	}

	ImGui::Columns(1);

	ImGui::End();
}

void ContentBrowserPanel::CreateFolder()
{
	std::string output{ "assets/" };

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("New Folder", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Enter Folder Name\n");

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, s_folderName.c_str(), sizeof(buffer));
		if (ImGui::InputText("##FolderName", buffer, sizeof(buffer)))
		{
			s_folderName = std::string(buffer);
			output += s_folderName;
		}

		ImGui::Spacing();
		
		if (ImGui::Button("Create", ImVec2(125.0f, 0))) 
		{ 
			output += s_folderName;
			if (!std::filesystem::is_directory(output))
			{
				std::filesystem::create_directory(output);
			}
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(125.0f, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void ContentBrowserPanel::DeleteFolder()
{

}
