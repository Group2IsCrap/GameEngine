#include "TileMap.h"
#include "Tile.h"
#include "../Events/EventDispatcher.h"
#include "../Graphics/AssetManager.h"
#include "../Graphics/GraphicsHandler.h"
#include "../Graphics/GraphicsEvents.h"

namespace Firelight::TileMap
{
    TileMap::TileMap()
        : m_tileHeight(1)
        , m_tileWidth(1)
        , m_tileMapHeight(50)
        , m_tileMapWidth(50)
        , m_tileDistance(1)
        , m_tileMap()
    {
        UpdateTileMapSize();

        newTilePtr = new Tile();
    }

    TileMap::~TileMap()
    {
        delete newTilePtr;
    }

    void TileMap::Render()
    {
        Events::EventDispatcher::SubscribeFunction<Events::Graphics::OnEarlyRender>(std::bind(&TileMap::DrawTiles, this));
    }

    void TileMap::UpdateTileMapSize()
    {
        unsigned int tileNum = 0;
        for (int x = 0; x < m_tileMapWidth; ++x)
        {
            std::vector<Tile*> vec;
            for (int y = 0; y < m_tileMapHeight; ++y)
            {
                  Tile* newTile = new Tile();
                  newTile->SetTileTexture(Graphics::AssetManager::Instance().GetDefaultTexture());
                  newTile->SetTileID(tileNum);
                  vec.emplace_back(newTile);

                  tileNum++;
            }
            m_tileMap.emplace_back(vec);
        }
    }

    void TileMap::DrawTiles()
    {
        int x = 0, y = 0;
        for (auto row = m_tileMap.begin(); row != m_tileMap.end(); ++row)
        {
            for (auto column = row->begin(); column != row->end(); ++column)
            {
                (*column)->SetDestinationRect(x * m_tileDistance, y * m_tileDistance, m_tileWidth, m_tileHeight);
                (*column)->DrawTile();
                ++x;
            }
            ++y;
            x = 0;
        }
    }

   void TileMap::SetTileHeight(int tileHeight)
    {
        m_tileHeight = tileHeight;
    }

    void TileMap::SetTileWidth(int tileWidth)
    {
        m_tileWidth = tileWidth;
    }

    void TileMap::SetTileMapHeight(int tileMapHeight)
    {
        m_tileMapHeight = tileMapHeight;
    }

    void TileMap::SetTileMapWidth(int tileMapWidth)
    {
        m_tileMapWidth = tileMapWidth;
    }

    Tile* TileMap::GetTileAtPosition(Maths::Vec2f position)
    {
        for (auto& tilesVec : m_tileMap)
        {
            for (auto tile : tilesVec)
            {
                if ((position.x > tile->GetDestinationRect().x) &&
                    (position.x < tile->GetDestinationRect().x + tile->GetDestinationRect().w) &&
                    (position.y > tile->GetDestinationRect().y) &&
                    (position.y < tile->GetDestinationRect().y + tile->GetDestinationRect().h))
                {
                    return tile;
                }
            }
        }
        newTilePtr->SetTileID(-1);
        return newTilePtr;
    }

    std::vector<std::vector<Tile*>>& TileMap::GetTileMap()
    {
        return m_tileMap;
    }
}
