#include "SpriteBatch.h"

#include "../Engine.h"

#include "AssetManager.h"
#include "Data/Texture.h"

#include "../Maths/Rect.h"
#include "../Maths/Vec2.h"

namespace Firelight::Graphics
{
    SpriteBatch::SpriteBatch() :
        m_queueLength(0),
        m_sortMode(SortMode::e_None)
    {
    }

    SpriteBatch::~SpriteBatch()
    {
        DeleteQueueData();
    }

    void SpriteBatch::SetSortMode(SortMode sortMode)
    {
        m_sortMode = sortMode;
    }

    void SpriteBatch::NDCDraw(const Maths::Rectf& destRectNDC, Texture* texture, int layer, double rotation, const Colour::RGBA& colour, const Maths::Rectf& sourceRect)
    {
        // If there is already an entry in the queue overwrite the existing data,
        // otherwise create push back new data
        Quad* quad = nullptr;
        if (m_queueLength < m_quadQueue.size())
        {
            quad = m_quadQueue[m_queueLength];
        }
        else
        {
            quad = new Quad();
            m_quadQueue.push_back(quad);
        }
        ++m_queueLength;
        
        // Use a blank white texture if none is specified
        if (texture != nullptr)
        {
            quad->m_texture = texture;
        }
        else
        {
            quad->m_texture = AssetManager::Instance().GetColourTexture(Colours::sc_white);
        }
        
        quad->m_layer = layer;
        float depthFromLayer = fminf(1.0f, (float)layer / 64.0f);

        // Calculate tex coords if weve been given a source rect
        if (sourceRect.IsValid())
        {
            quad->m_topLeft.m_texCoord = quad->m_texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x, sourceRect.y));
            quad->m_topRight.m_texCoord = quad->m_texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x + sourceRect.w, sourceRect.y));
            quad->m_bottomLeft.m_texCoord = quad->m_texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x, sourceRect.y + sourceRect.h));
            quad->m_bottomRight.m_texCoord = quad->m_texture->GetTexCoordFromSourcePixelCoord(Maths::Vec2f(sourceRect.x + sourceRect.w, sourceRect.y + sourceRect.h));
        }
        // Otherwise just use default texcoords
        else
        {
            quad->m_topLeft.m_texCoord = Maths::Vec2f(0.0f, 0.0f);
            quad->m_topRight.m_texCoord = Maths::Vec2f(1.0f, 0.0f);
            quad->m_bottomLeft.m_texCoord = Maths::Vec2f(0.0f, 1.0f);
            quad->m_bottomRight.m_texCoord = Maths::Vec2f(1.0f, 1.0f);
        }

        // If we are given a rotation, rotate the points
        Maths::Vec2f centre = destRectNDC.GetCentreNDC();
        if (rotation != 0.0)
        {
            const Maths::Vec2f& dimensions = Engine::Instance().GetWindowDimensionsFloat();
            float aspect = dimensions.x / dimensions.y;

            Maths::Vec2f rotatedTopLeft = Maths::Vec2f::RotateAroundPoint(Maths::Vec2f(destRectNDC.x, destRectNDC.y), centre, rotation, true, aspect);
            quad->m_topLeft.m_pos = Maths::Vec3f(rotatedTopLeft.x, rotatedTopLeft.y, depthFromLayer);

            Maths::Vec2f rotatedTopRight = Maths::Vec2f::RotateAroundPoint(Maths::Vec2f(destRectNDC.x + destRectNDC.w, destRectNDC.y), centre, rotation, true, aspect);
            quad->m_topRight.m_pos = Maths::Vec3f(rotatedTopRight.x, rotatedTopRight.y, depthFromLayer);

            Maths::Vec2f rotatedBottomLeft = Maths::Vec2f::RotateAroundPoint(Maths::Vec2f(destRectNDC.x, destRectNDC.y - destRectNDC.h), centre, rotation, true, aspect);
            quad->m_bottomLeft.m_pos = Maths::Vec3f(rotatedBottomLeft.x, rotatedBottomLeft.y, depthFromLayer);

            Maths::Vec2f rotatedBottomRight = Maths::Vec2f::RotateAroundPoint(Maths::Vec2f(destRectNDC.x + destRectNDC.w, destRectNDC.y - destRectNDC.h), centre, rotation, true, aspect);
            quad->m_bottomRight.m_pos = Maths::Vec3f(rotatedBottomRight.x, rotatedBottomRight.y, depthFromLayer);
        }
        else
        {
            quad->m_topLeft.m_pos = Maths::Vec3f(destRectNDC.x, destRectNDC.y, depthFromLayer);
            quad->m_topRight.m_pos = Maths::Vec3f(destRectNDC.x + destRectNDC.w, destRectNDC.y, depthFromLayer);
            quad->m_bottomLeft.m_pos = Maths::Vec3f(destRectNDC.x, destRectNDC.y - destRectNDC.h, depthFromLayer);
            quad->m_bottomRight.m_pos = Maths::Vec3f(destRectNDC.x + destRectNDC.w, destRectNDC.y - destRectNDC.h, depthFromLayer);
        }

        // Set quad vertex colours
        Maths::Vec4f floatColour = colour.GetAsNormalisedVec4();
        
        quad->m_topLeft.m_colour = floatColour;
        quad->m_topRight.m_colour = floatColour;
        quad->m_bottomLeft.m_colour = floatColour;
        quad->m_bottomRight.m_colour = floatColour;
    }

    void SpriteBatch::PixelDraw(const Maths::Rectf& destRectPixel, Texture* texture, int layer, double rotation, const Colour::RGBA& colour, const Maths::Rectf& sourceRect)
    {
        Maths::Rectf destRectNDC = destRectPixel.CreateNDCRectFromPixelRect();

        NDCDraw(destRectNDC, texture, layer, rotation, colour, sourceRect);
    }

    void SpriteBatch::WorldDraw(const Maths::Rectf& destRectWorld, Texture* texture, int layer, double rotation, const Colour::RGBA& colour, const Maths::Rectf& sourceRect)
    {
        // To do: convert from world space when we can
        NDCDraw(destRectWorld, texture, layer, rotation, colour, sourceRect);
    }

    void SpriteBatch::SortQuads()
    {
        switch (m_sortMode)
        {
        case SortMode::e_None:
            break;
        case SortMode::e_JustTexture:
            // Sort by texture.
            std::sort(m_quadQueue.begin(), m_quadQueue.begin() + m_queueLength,
                [](Quad const* x, Quad const* y) noexcept -> bool
                {
                    return x->m_texture < y->m_texture;
                });
            break;

        case SortMode::e_BackToFrontTexture:
            // Sort back to front.
            std::sort(m_quadQueue.begin(), m_quadQueue.begin() + m_queueLength,
                [](Quad const* x, Quad const* y) noexcept -> bool
                {
                    if (x->m_layer == y->m_layer)
                    {
                        return x->m_texture < y->m_texture;
                    }
                    else
                    {
                        return x->m_layer < y->m_layer;
                    }
                });
            break;

        case SortMode::e_FrontToBackTexture:
            // Sort front to back.
            std::sort(m_quadQueue.begin(), m_quadQueue.begin() + m_queueLength,
                [](Quad const* x, Quad const* y) noexcept -> bool
                {
                    if (x->m_layer == y->m_layer)
                    {
                        return x->m_texture < y->m_texture;
                    }
                    else
                    {
                        return x->m_layer > y->m_layer;
                    }
                });
            break;

        default:
            break;
        }
    }

    // Creates a mesh for each texture batch
    void SpriteBatch::CreateBatches()
    {
        m_spriteBatches.clear();

        SortQuads();

        std::vector<UnlitColourVertex> vertices;
        std::vector<DWORD> indices;
        Texture* batchTexture = nullptr;

        for (int quadIndex = 0; quadIndex < m_queueLength; ++quadIndex)
        {
            Quad* quad = m_quadQueue[quadIndex];

            // If the texture has changed, create a mesh for the
            // last batch and add it to the batches list
            if (batchTexture != quad->m_texture)
            {
                // If not the first quad
                if (batchTexture != nullptr)
                {
                    m_spriteBatches.push_back(Mesh(vertices, indices, batchTexture));
                    vertices.clear();
                    indices.clear();
                }

                batchTexture = quad->m_texture;
            }

            // Get vertex count before we add this quad
            DWORD vertexCount = (DWORD)vertices.size();

            vertices.push_back(quad->m_topLeft);
            vertices.push_back(quad->m_topRight);
            vertices.push_back(quad->m_bottomLeft);
            vertices.push_back(quad->m_bottomRight);

            // Clockwise winding
            indices.push_back(vertexCount);
            indices.push_back(vertexCount + 1);
            indices.push_back(vertexCount + 2);

            indices.push_back(vertexCount + 2);
            indices.push_back(vertexCount + 1);
            indices.push_back(vertexCount + 3);
        }

        // Add the last batch
        if (indices.size() > 0)
        {
            m_spriteBatches.push_back(Mesh(vertices, indices, batchTexture));
        }

        m_queueLength = 0;
    }

    const std::vector<Mesh>& SpriteBatch::GetBatches()
    {
        return m_spriteBatches;
    }

    void SpriteBatch::DeleteQueueData()
    {
        for (int quadIndex = 0; quadIndex < m_quadQueue.size(); ++quadIndex)
        {
            delete m_quadQueue[quadIndex];
        }

        m_quadQueue.clear();
        m_queueLength = 0;
    }
}
