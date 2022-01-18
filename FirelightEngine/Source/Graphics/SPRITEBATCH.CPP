#include "SpriteBatch.h"
#include "../Maths/Rect.h"
#include "../Maths/Vec2.h"

#include "Data/Texture.h"
#include "../Engine.h"

namespace Firelight::Graphics
{
    SpriteBatch::SpriteBatch()
    {
    }

    SpriteBatch::~SpriteBatch()
    {
    }

    // Creates a quad and adds it to map
    void SpriteBatch::NDCDraw(const Maths::Rectf& destRectNDC, const Maths::Rectf& sourceRect, int layer, Texture* texture)
    {
        float depthFromLayer = (float)layer / 64.0f;

        Quad quad;
        quad.m_depth = depthFromLayer;

        quad.m_topLeft.m_pos = Maths::Vec3f(destRectNDC.x, destRectNDC.y, depthFromLayer);
        quad.m_topLeft.m_texCoord = texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x, sourceRect.y));

        quad.m_topRight.m_pos = Maths::Vec3f(destRectNDC.x + destRectNDC.w, destRectNDC.y, depthFromLayer);
        quad.m_topRight.m_texCoord = texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x + sourceRect.w, sourceRect.y));

        quad.m_bottomLeft.m_pos = Maths::Vec3f(destRectNDC.x, destRectNDC.y - destRectNDC.h, depthFromLayer);
        quad.m_bottomLeft.m_texCoord = texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x, sourceRect.y + sourceRect.h));

        quad.m_bottomRight.m_pos = Maths::Vec3f(destRectNDC.x + destRectNDC.w, destRectNDC.y - destRectNDC.h, depthFromLayer);
        quad.m_bottomRight.m_texCoord = texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x, sourceRect.y + sourceRect.h));

        // Add quad and group next to other quads
        m_spriteQuadsMap[texture].push_back(quad);
    }

    void SpriteBatch::PixelDraw(const Maths::Rectf& destRectPixel, const Maths::Rectf& sourceRect, int layer, Texture* texture)
    {
        Maths::Rectf destRectNDC = destRectPixel.CreateNDCRectFromPixelRect();

        NDCDraw(destRectNDC, sourceRect, layer, texture);
    }

    void SpriteBatch::WorldDraw(const Maths::Rectf& destRectWorld, const Maths::Rectf& sourceRect, int layer, Texture* texture)
    {
        // To do: convert from world space when we can
        NDCDraw(destRectWorld, sourceRect, layer, texture);
    }

    // Creates a mesh for each texture
    void SpriteBatch::CreateBatches()
    {
        m_spriteBatches.clear();

        std::vector<UnlitVertex> vertices;
        std::vector<DWORD> indices;

        for (auto& spriteQuads : m_spriteQuadsMap)
        {
            vertices.clear();
            indices.clear();

            for (auto& spriteQuad : spriteQuads.second)
            {
                DWORD vertexCount = (DWORD)vertices.size();

                vertices.push_back(spriteQuad.m_topLeft);
                vertices.push_back(spriteQuad.m_topRight);
                vertices.push_back(spriteQuad.m_bottomLeft);
                vertices.push_back(spriteQuad.m_bottomRight);

                indices.push_back(vertexCount);
                indices.push_back(vertexCount + 1);
                indices.push_back(vertexCount + 2);

                indices.push_back(vertexCount + 2);
                indices.push_back(vertexCount + 1);
                indices.push_back(vertexCount + 3);
            }

            m_spriteBatches.push_back(Mesh(vertices, indices, spriteQuads.first));
        }

        m_spriteQuadsMap.clear();
    }

    const std::vector<Mesh>& SpriteBatch::GetBatches()
    {
        return m_spriteBatches;
    }
}
