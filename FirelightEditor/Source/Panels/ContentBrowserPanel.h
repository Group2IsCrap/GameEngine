#pragma once

#include "Panel.h"

#include <filesystem>
#include <Source/Graphics/Data/Texture.h>

class ContentBrowserPanel : public Panel
{
public:
	ContentBrowserPanel();

	void Draw() override;

private:
	std::filesystem::path m_currentDirectory;

	Firelight::Graphics::Texture* m_fileIcon;
	Firelight::Graphics::Texture* m_directoryIcon;
};

