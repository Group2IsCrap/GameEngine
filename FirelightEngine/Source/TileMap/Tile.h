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
		void SetSourceRect(float x, float y, float w, float h);
		void SetLayer(int layer);
		void SetRotation(double layer);

	private:
		Graphics::Texture* m_texture;
		Maths::Rectf m_sourceRect;
		Maths::Rectf m_destinationRect;
		int m_layer;
		double m_rotation;
	};
}
