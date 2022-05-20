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
		, m_destinationRect(-2.0f, -2.0f, 1.0f, 1.0f)
		, m_layer(32)
		, m_rotation(0.0)
	{}

	void Tile::DrawTile()
	{
		Graphics::GraphicsHandler::Instance().GetSpriteBatch()->WorldDraw(m_destinationRect, m_texture, m_layer, m_rotation, Firelight::Graphics::Colours::sc_white, m_sourceRect);
	}

	void Tile::SetTileTexture(Graphics::Texture* texture)
	{
		m_texture = texture;
	}

	void Tile::SetDestinationRect(float x, float y, float w, float h)
	{
		m_destinationRect = Maths::Rectf(x, y, w, h);
	}

	void Tile::SetSourceRect(float x, float y, float w, float h)
	{
		m_sourceRect = Maths::Rectf(x, y, w, h);
	}

	void Tile::SetLayer(int layer)
	{
		m_layer = layer;
	}

	void Tile::SetRotation(double rotation)
	{
		m_rotation = rotation;
	}
}