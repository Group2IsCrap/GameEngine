#pragma once
#include "Tile.h"

#include <vector>
#include <memory>
#include <map>
#include "../Maths/Rect.h"
#include "../Events/Listener.h"

namespace Firelight::TileMap
{

	class TileMap : public Firelight::Events::Listener
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

		Tile GetTileAtPosition(float x, float y);

		std::vector<std::vector<Tile>>& GetTileMap();

		virtual void HandleEvents(const char* event, void* data) override final;

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
		std::map<Maths::Vec2<int>, Tile> m_grid;

		std::unique_ptr<std::map<unsigned int, Maths::Rectf>> m_tileAtlas;
	};
}