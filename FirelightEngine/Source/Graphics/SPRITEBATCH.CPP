#include "SpriteBatch.h"

#include "../Engine.h"

#include "AssetManager.h"
#include "Data/Texture.h"

#include "../Maths/Rect.h"
#include "../Maths/Vec2.h"

#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"

namespace Firelight::Graphics
{
    SpriteBatch::SpriteBatch() :
        m_sortMode(SortMode::e_None)
    {
    }

    SpriteBatch::~SpriteBatch()
    {
    }

    void SpriteBatch::SetSortMode(SortMode sortMode)
    {
        m_sortMode = sortMode;
    }

    void SpriteBatch::NDCDraw(const Maths::Rectf& destRectNDC, Texture* texture, int layer, double rotation, const Colour::RGBA& colour, const Maths::Rectf& sourceRect, bool flipX, bool flipY)
    {
        // If there is already an entry in the queue overwrite the existing data,
        // otherwise create push back new data
        QuadEntity* quad = new QuadEntity();
        m_entityQueue.push_back(quad);
        
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
            quad->m_texture->GetTexCoordFromSourcePixelCoord(quad->m_topLeft.m_texCoord, Maths::Vec2f(sourceRect.x, sourceRect.y));
            quad->m_texture->GetTexCoordFromSourcePixelCoord(quad->m_topRight.m_texCoord, Maths::Vec2f(sourceRect.x + sourceRect.w, sourceRect.y));
            quad->m_texture->GetTexCoordFromSourcePixelCoord(quad->m_bottomLeft.m_texCoord, Maths::Vec2f(sourceRect.x, sourceRect.y + sourceRect.h));
            quad->m_texture->GetTexCoordFromSourcePixelCoord(quad->m_bottomRight.m_texCoord, Maths::Vec2f(sourceRect.x + sourceRect.w, sourceRect.y + sourceRect.h));
        }
        // Otherwise just use default texcoords
        else
        {
            quad->m_topLeft.m_texCoord = Maths::Vec2f(0.0f, 0.0f);
            quad->m_topRight.m_texCoord = Maths::Vec2f(1.0f, 0.0f);
            quad->m_bottomLeft.m_texCoord = Maths::Vec2f(0.0f, 1.0f);
            quad->m_bottomRight.m_texCoord = Maths::Vec2f(1.0f, 1.0f);
        }

        // Perform flips if necessary
        if (flipX)
        {
            // Make copies of the original tex coords
            Maths::Vec2f topLeftCoord = quad->m_topLeft.m_texCoord;
            Maths::Vec2f botLeftCoord = quad->m_bottomLeft.m_texCoord;

            // Do the actual flip
            quad->m_topLeft.m_texCoord = quad->m_topRight.m_texCoord;
            quad->m_topRight.m_texCoord = topLeftCoord;
            quad->m_bottomLeft.m_texCoord = quad->m_bottomRight.m_texCoord;
            quad->m_bottomRight.m_texCoord = botLeftCoord;
        }
        if (flipY)
        {
            // Make copies of the original tex coords
            Maths::Vec2f topLeftCoord = quad->m_topLeft.m_texCoord;
            Maths::Vec2f topRightCoord = quad->m_topRight.m_texCoord;

            // Do the actual flip
            quad->m_topLeft.m_texCoord = quad->m_bottomLeft.m_texCoord;
            quad->m_topRight.m_texCoord = quad->m_bottomRight.m_texCoord;
            quad->m_bottomLeft.m_texCoord = topLeftCoord;
            quad->m_bottomRight.m_texCoord = topRightCoord;
        }

        // If we are given a rotation, rotate the points
        Maths::Vec2f centre = destRectNDC.GetCentreNDC();
        if (rotation != 0.0)
        {
            const Maths::Vec2f& dimensions = Engine::Instance().GetWindowDimensionsFloat();
            float aspect = dimensions.x / dimensions.y;

            Maths::Vec2f rotatedTopLeft = Maths::Vec2f::RotateAroundPoint(Maths::Vec2f(destRectNDC.x, destRectNDC.y + destRectNDC.h), centre, rotation, true, aspect);
            quad->m_topLeft.m_pos = Maths::Vec3f(rotatedTopLeft.x, rotatedTopLeft.y, depthFromLayer);

            Maths::Vec2f rotatedTopRight = Maths::Vec2f::RotateAroundPoint(Maths::Vec2f(destRectNDC.x + destRectNDC.w, destRectNDC.y + destRectNDC.h), centre, rotation, true, aspect);
            quad->m_topRight.m_pos = Maths::Vec3f(rotatedTopRight.x, rotatedTopRight.y, depthFromLayer);

            Maths::Vec2f rotatedBottomLeft = Maths::Vec2f::RotateAroundPoint(Maths::Vec2f(destRectNDC.x, destRectNDC.y), centre, rotation, true, aspect);
            quad->m_bottomLeft.m_pos = Maths::Vec3f(rotatedBottomLeft.x, rotatedBottomLeft.y, depthFromLayer);

            Maths::Vec2f rotatedBottomRight = Maths::Vec2f::RotateAroundPoint(Maths::Vec2f(destRectNDC.x + destRectNDC.w, destRectNDC.y), centre, rotation, true, aspect);
            quad->m_bottomRight.m_pos = Maths::Vec3f(rotatedBottomRight.x, rotatedBottomRight.y, depthFromLayer);
        }
        else
        {
            quad->m_topLeft.m_pos = Maths::Vec3f(destRectNDC.x, destRectNDC.y + destRectNDC.h, depthFromLayer);
            quad->m_topRight.m_pos = Maths::Vec3f(destRectNDC.x + destRectNDC.w, destRectNDC.y + destRectNDC.h, depthFromLayer);
            quad->m_bottomLeft.m_pos = Maths::Vec3f(destRectNDC.x, destRectNDC.y, depthFromLayer);
            quad->m_bottomRight.m_pos = Maths::Vec3f(destRectNDC.x + destRectNDC.w, destRectNDC.y, depthFromLayer);
        }

        // Set quad vertex colours
        Maths::Vec4f floatColour = colour.GetAsNormalisedVec4();
        
        quad->m_topLeft.m_colour = floatColour;
        quad->m_topRight.m_colour = floatColour;
        quad->m_bottomLeft.m_colour = floatColour;
        quad->m_bottomRight.m_colour = floatColour;
    }

    void SpriteBatch::PixelDraw(const Maths::Rectf& destRectPixel, Texture* texture, int layer, double rotation, const Colour::RGBA& colour, const Maths::Rectf& sourceRect, bool flipX, bool flipY)
    {
        Maths::Rectf destRectNDC = destRectPixel.CreateNDCRectFromPixelRect(Engine::Instance().GetWindowDimensionsFloat());

        NDCDraw(destRectNDC, texture, layer, rotation, colour, sourceRect, flipX, flipY);
    }

    void SpriteBatch::WorldDraw(const Maths::Rectf& destRectWorld, Texture* texture, int layer, double rotation, const Colour::RGBA& colour, const Maths::Rectf& sourceRect, bool flipX, bool flipY)
    {
        Maths::Rectf destRectNDC = Maths::Rectf::CreateNDCRectInWorldRect(destRectWorld, Engine::Instance().GetActiveCamera2DRect());
 
        NDCDraw(destRectNDC, texture, layer, rotation, colour, sourceRect, flipX, flipY);
    }

    void SpriteBatch::DrawTextStandard(Text* text, int layer)
    {
        TextEntity* textEntity = new TextEntity();
        
        textEntity->m_text = text;
        textEntity->m_layer = layer;

        m_entityQueue.push_back(textEntity);
    }

    void SpriteBatch::SortQuads()
    {
        switch (m_sortMode)
        {
        case SortMode::e_None:
            break;
        case SortMode::e_JustTexture:
            // Sort by texture.
            std::sort(m_entityQueue.begin(), m_entityQueue.begin() + m_entityQueue.size(),
                [](LayerableEntity const* x, LayerableEntity const* y) noexcept -> bool
                {
                    if (auto const* quadX = dynamic_cast<QuadEntity const*>(x))
                    {
                        if (auto const* quadY = dynamic_cast<QuadEntity const*>(y))
                        {
                            return quadX->m_texture < quadY->m_texture;
                        }
                    }

                    return false;
                });
            break;

        case SortMode::e_BackToFrontTexture:
            // Sort back to front.
            std::sort(m_entityQueue.begin(), m_entityQueue.begin() + m_entityQueue.size(),
                [](LayerableEntity const* x, LayerableEntity const* y) noexcept -> bool
                {
                    if (x->m_layer == y->m_layer)
                    {
                        if (auto const* quadX = dynamic_cast<QuadEntity const*>(x))
                        {
                            if (auto const* quadY = dynamic_cast<QuadEntity const*>(y))
                            {
                                return quadX->m_texture < quadY->m_texture;
                            }
                        }

                        return false;
                    }
                    else
                    {
                        return x->m_layer < y->m_layer;
                    }
                });
            break;

        case SortMode::e_FrontToBackTexture:
            // Sort front to back.
            std::sort(m_entityQueue.begin(), m_entityQueue.begin() + m_entityQueue.size(),
                [](LayerableEntity const* x, LayerableEntity const* y) noexcept -> bool
                {
                    if (x->m_layer == y->m_layer)
                    {
                        if (auto const* quadX = dynamic_cast<QuadEntity const*>(x))
                        {
                            if (auto const* quadY = dynamic_cast<QuadEntity const*>(y))
                            {
                                return quadX->m_texture < quadY->m_texture;
                            }
                        }

                        return false;
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

    // Creates a mesh for each texture batch and draw it
    void SpriteBatch::CreateAndDrawBatches(bool bindPSData)
    {
        ID3D11DeviceContext* deviceContext = GraphicsHandler::Instance().GetDeviceContext();

        SortQuads();

        std::vector<UnlitColourVertex> vertices;
        std::vector<DWORD> indices;
        Texture* batchTexture = nullptr;

        auto drawCurrentBatch = [&]()
        {
            Mesh mesh = Mesh(vertices, indices, batchTexture);

            deviceContext->VSSetShader(AssetManager::Instance().GetVertexShader("$ENGINE/Shaders/Vertex/UnlitColour")->GetShader(), NULL, 0);
            deviceContext->PSSetShader(AssetManager::Instance().GetPixelShader("$ENGINE/Shaders/Pixel/UnlitColour")->GetShader(), NULL, 0);

            mesh.Draw(bindPSData);

            vertices.clear();
            indices.clear();
        };

        for (int entityIndex = 0; entityIndex < m_entityQueue.size(); ++entityIndex)
        {
            LayerableEntity* entity = m_entityQueue[entityIndex];

            if (auto* quadEntity = dynamic_cast<QuadEntity*>(entity))
            {
                // If the texture has changed, create a mesh for the
                // last batch and add it to the batches list
                if (batchTexture != quadEntity->m_texture)
                {
                    // If not the first quad
                    if (batchTexture != nullptr)
                    {
                        drawCurrentBatch();
                    }

                    batchTexture = quadEntity->m_texture;
                }

                // Get vertex count before we add this quad
                DWORD vertexCount = (DWORD)vertices.size();

                vertices.push_back(quadEntity->m_topLeft);
                vertices.push_back(quadEntity->m_topRight);
                vertices.push_back(quadEntity->m_bottomLeft);
                vertices.push_back(quadEntity->m_bottomRight);

                // Clockwise winding
                indices.push_back(vertexCount);
                indices.push_back(vertexCount + 1);
                indices.push_back(vertexCount + 2);

                indices.push_back(vertexCount + 2);
                indices.push_back(vertexCount + 1);
                indices.push_back(vertexCount + 3);
            }
            else if (auto* textEntity = dynamic_cast<TextEntity*>(entity))
            {
                // If not the first quad
                if (batchTexture != nullptr)
                {
                    drawCurrentBatch();
                }

                deviceContext->VSSetShader(AssetManager::Instance().GetVertexShader("$ENGINE/Shaders/Vertex/Text")->GetShader(), NULL, 0);
                deviceContext->PSSetShader(AssetManager::Instance().GetPixelShader("$ENGINE/Shaders/Pixel/Text")->GetShader(), NULL, 0);

                textEntity->m_text->Draw();
            }
        }

        DeleteQueueData();

        // Add the last batch
        if (indices.size() > 0)
        {
            drawCurrentBatch();
        }
    }

    void SpriteBatch::DeleteQueueData()
    {
        for (int entityIndex = 0; entityIndex < m_entityQueue.size(); ++entityIndex)
        {
            delete m_entityQueue[entityIndex];
        }
        m_entityQueue.clear();
    }
}
