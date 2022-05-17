#include "TilemapSystem.h"

#include "../Source/Events/EventDispatcher.h"
#include "../Source/Graphics/GraphicsEvents.h"
#include "../Source/Graphics/GraphicsHandler.h"
#include "../Source/Graphics/SpriteBatch.h"
#include "../Source/Maths/Rect.h"

#include <vector>

namespace Firelight::ECS
{
	TilemapSystem::TilemapSystem()
	{
		AddWhitelistComponent<Firelight::ECS::TilemapComponent>();

		Events::EventDispatcher::SubscribeFunction<Events::Graphics::OnEarlyRender>(std::bind(&TilemapSystem::RenderTilemaps, this));
	}

	void TilemapSystem::RenderTilemaps()
	{
		for (int i = 0; i < m_entities.size(); ++i)
		{
			Firelight::ECS::TilemapComponent* tilemap = m_entities[i]->GetComponent<Firelight::ECS::TilemapComponent>();
			if (!tilemap)
			{
				continue;
			}

			RenderTilemap(tilemap);
		}
	}
	
	void TilemapSystem::RenderTilemap(Firelight::ECS::TilemapComponent* tilemap)
	{
		for (auto it = tilemap->map.begin(); it != tilemap->map.end(); ++it)
		{
			Firelight::Maths::Rectf destinationRect(it->first.first * tilemap->cellSize + 100.0f, it->first.second * tilemap->cellSize + 100.0f, tilemap->cellSize, tilemap->cellSize);
			Firelight::Maths::Rectf sourceRect(it->second->m_x * (tilemap->sourceSize + tilemap->sourceSpacing), it->second->m_y * (tilemap->sourceSize + tilemap->sourceSpacing), tilemap->sourceSize, tilemap->sourceSize);
			Graphics::GraphicsHandler::Instance().GetSpriteBatch()->PixelDraw(destinationRect, tilemap->Texture, it->second->m_layer, 0.0f, Firelight::Graphics::Colours::sc_white, sourceRect);
		}
	}
}
