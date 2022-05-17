#pragma once
#include "../Graphics/Data/Texture.h"
#include "../Maths/Rect.h"

namespace Firelight::TileMap
{
	struct Tile
	{
		int m_x;
		int m_y;
		int m_layer;

		Tile()
		{
			m_x = 0;
			m_y = 0;
			m_layer = 0;
		}

		Tile(int x, int y, int layer)
		{
			m_x = x;
			m_y = y;
			m_layer = layer;
		}
	};
}
