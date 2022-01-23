#pragma once

#include "../Source/Graphics/Data/Texture.h"
#include "../Source/Maths/Vec2.h"

#include <string>

namespace Firelight::Animation
{
	struct Animation
	{
		Animation() {}
		Animation(std::string name, Firelight::Graphics::Texture* texture, int cellWidth, int cellHeight, int frameCount, float frameTime = 100.0f, bool loop = true) : m_animationName(name), m_texture(texture), m_cellWidth(cellWidth), m_cellHeight(cellHeight), m_frameCount(frameCount),
			m_frameTime(frameTime), m_loop(loop)
		{
			Firelight::Maths::Vec3i textureSize = m_texture->GetDimensions();
			m_rows = static_cast<int>(std::floor(textureSize.x / m_cellWidth));
			m_columns = static_cast<int>(std::floor(textureSize.y / m_cellHeight));
		}

		std::string m_animationName;
		Firelight::Graphics::Texture* m_texture;
		float m_frameTime = 100;
		int m_cellWidth = 24;
		int m_cellHeight = 24;
		int m_frameCount = 1;
		bool m_loop = true;
		
		int m_rows = 0;
		int m_columns = 0;

	};
}