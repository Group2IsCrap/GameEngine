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
        , m_bottomLeftTilePos(Maths::Vec2f(0.0f, 0.0f))
        , m_tileMap()
    {
        UpdateTileMapSize();

        m_emptyTilePtr = new Tile();
    }

    TileMap::~TileMap()
    {
        delete m_emptyTilePtr;
    }

    void TileMap::Render()
    {
        Events::EventDispatcher::SubscribeFunction<Events::Graphics::OnEarlyRender>(std::bind(&TileMap::DrawTiles, this));
    }

    void TileMap::UpdateTileMapSize()
    {
        unsigned int tileNum = 0;
        for (int x = m_bottomLeftTilePos.x; x < m_tileMapWidth + m_bottomLeftTilePos.x; ++x)
        {
            std::vector<Tile*> vec;
            for (int y = m_bottomLeftTilePos.y; y < m_tileMapHeight + m_bottomLeftTilePos.y; ++y)
            {
                  Tile* newTile = new Tile();
                  newTile->SetTileTexture(Graphics::AssetManager::Instance().GetDefaultTexture());
                  newTile->SetDestinationRect(x * m_tileDistance, y * m_tileDistance, m_tileWidth, m_tileHeight);
                  newTile->SetTileID(tileNum);
                  vec.emplace_back(newTile);

                  tileNum++;
            }
            m_tileMap.emplace_back(vec);
        }
    }

    void TileMap::DrawTiles()
    {
        int x = m_bottomLeftTilePos.x, y = m_bottomLeftTilePos.y;
        for (auto row = m_tileMap.begin(); row != m_tileMap.end(); ++row)
        {
            for (auto column = row->begin(); column != row->end(); ++column)
            {
                (*column)->SetDestinationRect(x * m_tileDistance, y * m_tileDistance, m_tileWidth, m_tileHeight);
                (*column)->DrawTile();
                ++x;
            }
            ++y;
            x = m_bottomLeftTilePos.x;
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
                if (position.x >= tile->GetDestinationRect().x)
                {
                    if (position.x <= tile->GetDestinationRect().x + tile->GetDestinationRect().w)
                    {
                        if ((position.y >= tile->GetDestinationRect().y))
                        {
                            if ((position.y <= tile->GetDestinationRect().y + tile->GetDestinationRect().h))
                            {
                                return tile;
                            }
                        }
                    }
                }
            }
        }
        m_emptyTilePtr->SetTileID(-1);
        return m_emptyTilePtr;
    }

    std::vector<std::vector<Tile*>>& TileMap::GetTileMap()
    {
        return m_tileMap;
    }

    void TileMap::SetBottomLeftTilePos(Firelight::Maths::Vec2f position)
    {
        m_bottomLeftTilePos = position;
    }
}
