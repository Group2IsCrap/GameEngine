#pragma once
#include <filesystem>
#include <string>
#include <Source/Graphics/Data/Texture.h>

#include "../Includes/imgui/imgui.h"

class ContentBrowserPanel
{
public:
	ContentBrowserPanel();

	void Draw();

private:
	void CreateFolder();
	void RemoveFile(std::filesystem::path path);

private:
	std::filesystem::path m_currentDirectory;

	Firelight::Graphics::Texture* m_fileIcon;
	Firelight::Graphics::Texture* m_directoryIcon;
};

