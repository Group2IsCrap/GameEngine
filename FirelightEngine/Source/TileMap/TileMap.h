#pragma once
#include "Tile.h"

#include <vector>
#include <memory>
#include <map>
#include "../Maths/Rect.h"

template<typename Iterator, typename UnaryPredicate>
auto find_if_in_2DArray(
	Iterator begin, const Iterator end, UnaryPredicate unarayPred) noexcept
{
	// used the standard algorithm std::find_if here!
	return std::find_if(begin, end, [unarayPred](const auto& row) noexcept {
		return std::find_if(std::cbegin(row), std::cend(row), unarayPred) != std::cend(row);
		}
	);
}


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

		Tile* GetTileAtPosition(Maths::Vec2f position);

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