#include "ContentBrowserPanel.h"

#include "Source/Graphics/AssetManager.h"
#include "Source/Graphics/Data/Texture.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

#include <filesystem>

static const std::filesystem::path s_assetPath = "Assets";

#define _CRT_SECURE_NO_WARNINGS

static std::string s_folderName;
static bool s_removeItem;
static bool s_newFolder;
static std::filesystem::path s_itemToDeletePath;
static std::filesystem::path s_newFolderPath;

ContentBrowserPanel::ContentBrowserPanel() : m_currentDirectory(s_assetPath)
{
	m_directoryIcon = Firelight::Graphics::AssetManager::Instance().GetTexture("$EDITOR/Icons/DirectoryIcon.png");
	m_fileIcon = Firelight::Graphics::AssetManager::Instance().GetTexture("$EDITOR/Icons/FileIcon.png");
}

void ContentBrowserPanel::Draw()
{
	ImGui::Begin("Content Browser");

	bool showNewFolder = false;
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		showNewFolder = true;
		if (ImGui::BeginMenu("New"))
		{
			if (ImGui::MenuItem("Folder"))
			{
				s_newFolder = true;
			}

			ImGui::EndMenu();
		}
		ImGui::EndPopup();
	}

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

	int count = 1;
	for (auto& directoryEntry : std::filesystem::directory_iterator(m_currentDirectory))
	{
		const auto& path = directoryEntry.path();

		std::string lowerPath = path.string();
		Firelight::Utils::StringHelpers::StringToLower(lowerPath);
		if (lowerPath.starts_with("assets\\$"))
			continue;

		auto relativePath = std::filesystem::relative(path, s_assetPath);
		std::string filenameString = relativePath.filename().string();

		std::string numberString = std::to_string(count);
		const char* itemNumber = numberString.c_str();

		Firelight::Graphics::Texture* icon;

		std::string extension = path.extension().string();
		if (extension == ".png" || extension == ".jpg")
		{
			std::string filePath = path.string();
			filePath.erase(0, 7);
			icon = Firelight::Graphics::AssetManager::Instance().GetTexture(filePath);
		}
		else
		{
			icon = directoryEntry.is_directory() ? m_directoryIcon : m_fileIcon;
		}
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushID(itemNumber);
		ImGui::ImageButton((ImTextureID)icon->GetShaderResourceView().Get(), { thumbnailSize, thumbnailSize }, { 0, 0 }, { 1, 1 },
			1, { 0, 0, 0, 0 }, { 1, 1, 1, 1 });

		// Right-click on blank space
		const char* removeFileID = "2";
		s_removeItem = false;

		if (!showNewFolder)
		{
			if (ImGui::BeginPopupContextItem(itemNumber, 1))
			{
				if (ImGui::MenuItem("Delete"))
				{
					s_removeItem = true;
					s_itemToDeletePath = path;
				}
				ImGui::EndPopup();
			}
		}

		ImGui::PopID();

		if (ImGui::BeginDragDropSource())
		{
			const wchar_t* itemPath = relativePath.c_str();
			ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
			ImGui::Image((ImTextureID)icon->GetShaderResourceView().Get(), { 50, 50 });
			ImGui::Text(filenameString.c_str());
			ImGui::EndDragDropSource();
		}

		if (s_newFolder)
		{
			s_newFolderPath = path;
		}

		if (s_removeItem)
		{
			RemoveFile(s_itemToDeletePath);
		}

		ImGui::PopStyleColor();
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
				m_currentDirectory /= path.filename();

		}
		ImGui::TextWrapped(filenameString.c_str());

		ImGui::NextColumn();

		count++;
	}

	if (s_newFolder)
	{
		ImGui::OpenPopup("New Folder");
	}

	CreateFolder();

	ImGui::Columns(1);

	ImGui::End();
}

void ContentBrowserPanel::CreateFolder()
{
	//std::string output{ "assets/" };

	// Always center this window when appearing
	ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	if (ImGui::BeginPopupModal("New Folder", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		s_newFolder = false;
		ImGui::Text("Enter Folder Name\n");

		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, s_folderName.c_str(), sizeof(buffer));
		if (ImGui::InputText("##FolderName", buffer, sizeof(buffer)))
		{
			s_folderName = std::string(buffer);
		}

		std::string newPath = m_currentDirectory.string() + "/" + s_folderName;

		ImGui::Spacing();
		
		if (ImGui::Button("Create", ImVec2(125.0f, 0))) 
		{ 
			s_newFolderPath += s_folderName;
			if (!std::filesystem::is_directory(newPath))
			{
				std::filesystem::create_directory(newPath);
				s_folderName = "";
			}
			ImGui::CloseCurrentPopup(); 
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(125.0f, 0))) { ImGui::CloseCurrentPopup(); s_newFolder = false; s_folderName = ""; }
		ImGui::EndPopup();
	}
}

void ContentBrowserPanel::RemoveFile(std::filesystem::path path)
{
	s_removeItem = false;
	if (std::filesystem::exists(path))
	{
		std::filesystem::remove_all(path);
	}
}
