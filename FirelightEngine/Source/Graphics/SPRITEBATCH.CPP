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

    void SpriteBatch::NDCDraw(const Maths::Rectf& destRectNDC, Texture* texture, int layer, const Maths::Rectf& sourceRect)
    {
        Quad quad;
        float depthFromLayer = (float)layer / 64.0f;

        // Calculate tex coords if weve been given a source rect
        if (sourceRect.IsValid())
        {
            quad.m_topLeft.m_texCoord = texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x, sourceRect.y));
            quad.m_topRight.m_texCoord = texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x + sourceRect.w, sourceRect.y));
            quad.m_bottomLeft.m_texCoord = texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x, sourceRect.y + sourceRect.h));
            quad.m_bottomRight.m_texCoord = texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x + sourceRect.w, sourceRect.y + sourceRect.h));
        }
        // Otherwise just use default texcoords
        else
        {
            quad.m_topLeft.m_texCoord = Maths::Vec2f(0.0f, 0.0f);
            quad.m_topRight.m_texCoord = Maths::Vec2f(1.0f, 0.0f);
            quad.m_bottomLeft.m_texCoord = Maths::Vec2f(0.0f, 1.0f);
            quad.m_bottomRight.m_texCoord = Maths::Vec2f(1.0f, 1.0f);
        }

        quad.m_topLeft.m_pos = Maths::Vec3f(destRectNDC.x, destRectNDC.y, depthFromLayer);
        quad.m_topRight.m_pos = Maths::Vec3f(destRectNDC.x + destRectNDC.w, destRectNDC.y, depthFromLayer);
        quad.m_bottomLeft.m_pos = Maths::Vec3f(destRectNDC.x, destRectNDC.y - destRectNDC.h, depthFromLayer);
        quad.m_bottomRight.m_pos = Maths::Vec3f(destRectNDC.x + destRectNDC.w, destRectNDC.y - destRectNDC.h, depthFromLayer);

        // Add new spritequads map entry for texture if it hasn't been drawn yet this frame
        // and add the quad to it's vector
        m_spriteQuadsMap[texture].push_back(quad);
    }

    void SpriteBatch::PixelDraw(const Maths::Rectf& destRectPixel, Texture* texture, int layer, const Maths::Rectf& sourceRect)
    {
        Maths::Rectf destRectNDC = destRectPixel.CreateNDCRectFromPixelRect();

        NDCDraw(destRectNDC, texture, layer, sourceRect);
    }

    void SpriteBatch::WorldDraw(const Maths::Rectf& destRectWorld, Texture* texture, int layer, const Maths::Rectf& sourceRect)
    {
        // To do: convert from world space when we can
        NDCDraw(destRectWorld, texture, layer, sourceRect);
    }

    // Creates a mesh for each texture batch
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

                // Clockwise winding
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
