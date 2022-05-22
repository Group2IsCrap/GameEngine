#pragma once
#include "../Graphics/Data/Texture.h"
#include "../Maths/Rect.h"

namespace Firelight::TileMap
{
	//struct Tile
	//{
	//	int m_x;
	//	int m_y;
	//	int m_layer;

	//	Tile()
	//	{
	//		m_x = 0;
	//		m_y = 0;
	//		m_layer = 0;
	//	}

	//	Tile(int x, int y, int layer)
	//	{
	//		m_x = x;
	//		m_y = y;
	//		m_layer = layer;
	//	}
	//};


	class Tile
	{
	public:
		Tile();
		void DrawTile();

		void SetTileTexture(Graphics::Texture* texture);
		void SetDestinationRect(float x, float y, float w, float h);
		void SetSourceRect(float x, float y, float w, float h);
		void SetLayer(int layer);
		void SetRotation(double layer);
		void SetTileID(unsigned int m_tileID);

		unsigned int GetTileID() const;
		Maths::Rectf GetDestinationRect() const;
		bool IsOccupied() const;

		bool test;

	private:

		Graphics::Texture* m_texture;
		Maths::Rectf m_sourceRect;
		Maths::Rectf m_destinationRect;
		unsigned int m_tileID;
		int m_layer;
		int m_rotation;
		bool m_isOccupied;
	};
}
