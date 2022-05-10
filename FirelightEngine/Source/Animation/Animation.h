#pragma once

#include "../Source/Graphics/Data/Texture.h"
#include "../Source/Maths/Vec2.h"

#include <string>
#include <math.h>
#include <vector>

namespace Firelight::Animation
{
	struct Animation
	{
		Animation() 
		{
			m_path = "";
			m_frameCount = 0;
			m_textures.clear();
		}

		std::string m_animationName;
		Firelight::Graphics::Texture* m_texture = nullptr;
		float m_frameTime = 100;
		int m_cellWidth = 24;
		int m_cellHeight = 24;
		int m_frameCount = 0;
		bool m_loop = true;
		
		int m_rows = 0;
		int m_columns = 0;

		std::vector<std::string> m_textureNames;
		std::vector<Firelight::Graphics::Texture*> m_textures;
		std::string m_path;
		int m_index = 0;

	};
}