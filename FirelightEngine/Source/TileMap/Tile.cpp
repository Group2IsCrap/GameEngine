#include "Tile.h"
#include "../Graphics/GraphicsHandler.h"
#include "../Graphics/SpriteBatch.h"
#include "../Events/EventDispatcher.h"
#include "../Graphics/GraphicsEvents.h"
#include "../Graphics/AssetManager.h"

namespace Firelight::TileMap
{
	Tile::Tile()
		: m_texture(Graphics::AssetManager::Instance().GetDefaultTexture())
		, m_sourceRect(0.0f, 0.0f, -1.0f, -1.0f)
		, m_destinationRect(0.0f, 0.0f, 1.0f, 1.0f)
		, m_tileID(-1)
		, m_layer(32)
		, m_rotation(0.0)
		, m_isDrawn(false)
		, m_isOccupied(false)
	{}

	void Tile::DrawTile()
	{
		if (m_isDrawn)
		{
			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(m_destinationRect, m_texture, m_layer, m_rotation, Firelight::Graphics::Colours::sc_white, m_sourceRect);
		}
	}

	void Tile::SetTileTexture(Graphics::Texture* texture)
	{
		m_texture = texture;
	}

	void Tile::SetDestinationRect(float x, float y, float w, float h)
	{
		m_destinationRect = Maths::Rectf(x, y, w, h);
	}

	void Tile::SetTileID(unsigned int tileID)
	{
		m_tileID = tileID;
	}

	void Tile::SetIsDrawn(bool isDrawn)
	{
		m_isDrawn = isDrawn;
	}

	void Tile::SetIsOccupied(bool occupied)
	{
		m_isOccupied = occupied;
	}

	unsigned int Tile::GetTileID() const
	{
		return m_tileID;
	}

	Maths::Rectf Tile::GetDestinationRect() const
	{
		return m_destinationRect;
	}


	bool Tile::IsOccupied() const
	{
		return m_isOccupied;
	}
	bool Tile::IsDrawn() const
	{
		return m_isDrawn;
	}
}