#pragma once
#include "../Graphics/Data/Texture.h"
#include "../Maths/Rect.h"

namespace Firelight::TileMap
{
	class Tile
	{
	public:
		Tile();
		void DrawTile();

		void SetTileTexture(Graphics::Texture* texture);
		void SetDestinationRect(float x, float y, float w, float h);
		void SetTileID(int m_tileID);
		void SetIsDrawn(bool isDrawn);
		void SetIsOccupied(bool occupied);

		unsigned int GetTileID() const;
		Maths::Rectf GetDestinationRect() const;
		bool IsOccupied() const;
		bool IsDrawn() const;

	private:

		Graphics::Texture* m_texture;
		Maths::Rectf m_sourceRect;
		Maths::Rectf m_destinationRect;
		int m_tileID;
		int m_layer;
		int m_rotation;
		bool m_isDrawn;
		bool m_isOccupied;
	};
}
