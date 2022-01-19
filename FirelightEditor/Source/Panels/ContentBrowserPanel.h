#pragma once

#include "Panel.h"

#include <filesystem>
#include <string>
#include <Source/Graphics/Data/Texture.h>

class ContentBrowserPanel : public Panel
{
public:
	ContentBrowserPanel();

	void Draw() override;

private:
	void CreateFolder();
	void DeleteFolder();

private:
	std::filesystem::path m_currentDirectory;

	Firelight::Graphics::Texture* m_fileIcon;
	Firelight::Graphics::Texture* m_directoryIcon;
};

