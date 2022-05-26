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
        , m_tileMapHeight(200)
        , m_tileMapWidth(200)
        , m_tileDistance(1)
        , m_tileMapSize(m_tileMapHeight * m_tileMapWidth)
        , m_bottomLeftTilePos(Maths::Vec2f(0.0f, 0.0f))
        , m_topRightTilePos(Maths::Vec2f(0.0f, 0.0f))
        , m_tileMap()
    {
        PopulateTileMap();

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

    void TileMap::PopulateTileMap()
    {
        unsigned int tileNum = 0;
        for (int x = m_bottomLeftTilePos.x; x < m_tileMapWidth + m_bottomLeftTilePos.x; ++x)
        {
            std::vector<Tile*> vec;
            for (int y = m_bottomLeftTilePos.y; y < m_tileMapHeight + m_bottomLeftTilePos.y; ++y)
            {
                  Tile* newTile = new Tile();
                  newTile->SetDestinationRect(x * m_tileDistance, y * m_tileDistance, m_tileWidth, m_tileHeight);
                  newTile->SetTileID(tileNum);
                  vec.emplace_back(newTile);

                  tileNum++;
            }
            m_tileMap.emplace_back(vec);
        }
    }

    void TileMap::UpdateTileMapPositions()
    {
        int x = 0, y = 0;
        for (auto row = m_tileMap.begin(); row != m_tileMap.end(); ++row)
        {
            for (auto column = row->begin(); column != row->end(); ++column)
            {
                (*column)->SetDestinationRect(x + m_bottomLeftTilePos.x * m_tileDistance, y + m_bottomLeftTilePos.y * m_tileDistance, m_tileWidth, m_tileHeight);
                ++x;
            }
            ++y;
            x = m_bottomLeftTilePos.x;
        }
    }

    void TileMap::DrawTiles()
    {
        for (size_t rowIndex = 0; rowIndex < m_tileMap.size(); ++rowIndex)
        {
            auto& rowObject = m_tileMap[rowIndex];

            for (size_t columnIndex = 0; columnIndex < rowObject.size(); ++columnIndex)
            {
                rowObject[columnIndex]->DrawTile();
            }
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

    void TileMap::SetTileMap(std::vector<std::vector<Tile*>> tileMap)
    {
        m_tileMap = tileMap;
    }

    Tile* TileMap::GetTileAtPosition(Maths::Vec2f position)
    {
        for (auto& tilesVec : m_tileMap)
        {
            for (auto tile : tilesVec)
            {
                Firelight::Maths::Rectf rect = tile->GetDestinationRect();
                if ((position.x >= rect.x) &&
                    (position.x <= rect.x + rect.w) &&
                    (position.y >= rect.y) &&
                    (position.y <= rect.y + rect.h))
                {
                    return tile;
                }
            }
        }
        m_emptyTilePtr->SetTileID(0);
        return m_emptyTilePtr;
    }

    std::vector<std::vector<Tile*>>& TileMap::GetTileMap()
    {
        return m_tileMap;
    }

    void TileMap::SetBottomLeftTilePos(Firelight::Maths::Vec2f position)
    {
        m_bottomLeftTilePos = position;

        m_topRightTilePos.x = (m_bottomLeftTilePos.x + m_tileMapWidth) * m_tileWidth;
        m_topRightTilePos.y = (m_bottomLeftTilePos.y + m_tileMapHeight) * m_tileHeight;

    }

    Firelight::Maths::Vec2f TileMap::GetBottomLeftTilePos()
    {
        return m_bottomLeftTilePos;
    }
    Firelight::Maths::Vec2f TileMap::GetTopRightTilePos()
    {
        return m_topRightTilePos;
    }

    size_t TileMap::GetTileMapSize()
    {
        return m_tileMapSize;
    }
}
