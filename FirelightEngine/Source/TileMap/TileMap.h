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
		enum class Scale
		{
			Positive,
			Center,
			Negative
		};

		TileMap();
		void Render();

		void UpdateTileMapSize();
		void DrawTiles();

		void SetTileHeight(float tileHeight);
		void SetTileWidth(float tileWidth);
		void SetTileMapHeight(float tileMapHeight);
		void SetTileMapWidth(float tileMapWidth);

		std::vector<std::vector<Tile>>& GetTileMap();

	private:
		float m_tileHeight;
		float m_tileWidth;
		float m_tileMapHeight;
		float m_tileMapWidth;
		float m_tileDistance;

		Scale em_VerticalScale;
		Scale em_HorizontalScale;

		std::vector<std::vector<Tile>> m_tileMap;
		std::vector<std::vector<Tile>> m_newTileMap;

		std::unique_ptr<std::map<unsigned int, Maths::Rectf>> m_tileAtlas;
	};
}