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
		~TileMap();
		void Render();

		void UpdateTileMapSize();
		void DrawTiles();

		void SetTileHeight(int tileHeight);
		void SetTileWidth(int tileWidth);
		void SetTileMapHeight(int tileMapHeight);
		void SetTileMapWidth(int tileMapWidth);

		void SetBottomLeftTilePos(Firelight::Maths::Vec2f position);

		Tile* GetTileAtPosition(Maths::Vec2f position);

		std::vector<std::vector<Tile*>>& GetTileMap();

	private:
		int m_tileHeight;
		int m_tileWidth;
		int m_tileMapHeight;
		int m_tileMapWidth;
		int m_tileDistance;

		Firelight::Maths::Vec2f m_bottomLeftTilePos;

		std::vector<std::vector<Tile*>> m_tileMap;

		Tile* m_emptyTilePtr;
	};
}