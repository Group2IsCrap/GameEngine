#pragma once

#include "../Source/Graphics/Data/Texture.h"

#include <string>

namespace Firelight::Animation
{
	struct Animation
	{
		std::string m_animationName;
		Firelight::Graphics::Texture* m_texture;
		int m_frameTime = 100;
		int m_cellWidth = 24;
		int m_cellHeight = 24;
		int m_frameCount = 1;
		bool m_loop = true;
		
		int m_rows;
		int m_columns;
	};
}