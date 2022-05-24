#pragma once

#include "../ECSDefines.h"
#include "../../Graphics/Data/Texture.h"
#include "../../TileMap/Tile.h"

#include <map>

namespace Firelight::ECS
{
	struct TilemapComponent : BaseComponent
	{
		int width = 100;
		int height = 100;
		int sourceSize = 16;
		int sourceSpacing = 2;
		int cellSize = 16;
		int pixelPerUnit = 1;
		Firelight::Graphics::Texture* texture;
		std::map<std::pair<int, int>, Firelight::TileMap::Tile*> map;


		void Serialise() override
		{
			//Serialiser::Serialise("Width", width);
		}

		TilemapComponent* Clone() override
		{
			TilemapComponent* clone = new TilemapComponent();
			clone->width = width;
			clone->height = height;
			clone->sourceSize = sourceSize;
			clone->sourceSpacing = sourceSpacing;
			clone->cellSize = cellSize;
			clone->pixelPerUnit = pixelPerUnit;
			clone->texture = texture;
			clone->map = map;


			return clone;
		}

		void RenderDebugUI() override
		{
			ImGuiText("TilemapComponent");
		}
	};
}