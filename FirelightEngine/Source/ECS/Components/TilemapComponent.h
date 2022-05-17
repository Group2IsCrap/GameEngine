#pragma once

#include "../ECSDefines.h"
#include "../../Graphics/Data/Texture.h"
#include "../../TileMap/Tile.h"

#include <map>

namespace Firelight::ECS
{
	class TilemapComponent : public BaseComponent
	{
	public:
		int width = 100;
		int height = 100;
		int sourceSize = 16;
		int sourceSpacing = 2;
		int cellSize = 16;
		int pixelPerUnit = 1;
		Firelight::Graphics::Texture* Texture;
		std::map<std::pair<int, int>, Firelight::TileMap::Tile*> map;
	};
}