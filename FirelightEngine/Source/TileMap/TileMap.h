#pragma once
#include "Tile.h"

#include <vector>
#include <memory>
#include <map>
#include "../Maths/Rect.h"

namespace Firelight::TileMap
{

	class TileMap
	{
	public:

		TileMap();
		void Render();

		void UpdateTileMapSize();
		void DrawTiles();

		void SetTileHeight(int tileHeight);
		void SetTileWidth(int tileWidth);
		void SetTileMapHeight(int tileMapHeight);
		void SetTileMapWidth(int tileMapWidth);

		std::vector<std::vector<Tile>>& GetTileMap();

	private:
		int m_tileHeight;
		int m_tileWidth;
		int m_tileMapHeight;
		int m_tileMapWidth;
		int m_tileDistance;

		std::vector<std::vector<Tile>> m_tileMap;
		std::vector<std::vector<Tile>> m_newTileMap;
	};
}