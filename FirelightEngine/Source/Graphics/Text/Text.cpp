#include "Text.h"

#include <vector>

#include "../Data/VertexTypes.h"

#include "../AssetManager.h"

#include "../../Engine.h"

namespace Firelight::Graphics
{
    Firelight::Graphics::Text::Text::Text(const std::string& string, Font* font) :
        m_string(string),
        m_font(font),
        
        m_anchorPosition(Maths::Vec2f(0.0f, 0.0f)),
        m_textSpace(TextSpace::e_Pixel),
        m_textAnchor(TextAnchor::e_TopLeft),
        m_textAlign(TextAlign::e_Left),
        
        m_textHeight(250.0f),
        m_spacingMod(0.7f),
        m_maxLineWidth(0.0f),
        
        m_mesh(nullptr),
        m_textCB()
    {

        if (m_font == nullptr)
        {
            m_font = AssetManager::Instance().GetDefaultFont();
        }

        // Create cb and default values
        m_textCB.Initialize(GraphicsHandler::Instance().GetDevice(), GraphicsHandler::Instance().GetDeviceContext());

        m_textCB.m_data.m_antiAliasDistanceRange = 0.005f;
        m_textCB.m_data.m_colour = Maths::Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
        m_textCB.m_data.m_outlineColour = Maths::Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

        m_textCB.m_data.m_scale = Maths::Vec2f(1.0f, 1.0f);
        m_textCB.m_data.m_positionOffset = Maths::Vec2f(0.0f, 0.0f);

        m_textCB.m_data.m_thickness = 0.5f;
        m_textCB.m_data.m_hasOutline = 1;
        m_textCB.m_data.m_outlineWidth = 0.35f;
    }

    Firelight::Graphics::Text::Text::~Text()
    {
    }

    void Text::Draw() const
    {
        GraphicsHandler::Instance().GetDeviceContext()->VSSetConstantBuffers(1, 1, m_textCB.GetAddressOf());
        GraphicsHandler::Instance().GetDeviceContext()->PSSetConstantBuffers(1, 1, m_textCB.GetAddressOf());

        m_mesh->Draw();
    }

    const Mesh* Firelight::Graphics::Text::Text::GetMesh() const
    {
        return m_mesh;
    }

    std::string Text::GetString()
    {
        return m_string;
    }

    void Text::SetString(const std::string& string, bool updateMesh)
    {
        m_string = string;

        if (updateMesh)
        {
            UpdateMesh();
        }
    }

    void Text::SetFont(Font* font, bool updateMesh)
    {
        m_font = font;

        if (updateMesh)
        {
            UpdateMesh();
        }
    }

    void Text::SetColour(const Colour::RGBA& colour, bool updateCB)
    {
        m_textCB.m_data.m_colour = colour.GetAsNormalisedVec4();

        if (updateCB)
        {
            m_textCB.MapToGPU();
        }
    }

    void Text::SetOutlineEnabled(bool enabled, bool updateCB)
    {
        m_textCB.m_data.m_hasOutline = (int)enabled;

        if (updateCB)
        {
            m_textCB.MapToGPU();
        }
    }

    void Text::SetOutlineWidth(float width, bool updateCB)
    {
        m_textCB.m_data.m_outlineWidth = width;

        if (updateCB)
        {
            m_textCB.MapToGPU();
        }
    }

    void Text::SetOutlineColour(const Colour::RGBA& colour, bool updateCB)
    {
        m_textCB.m_data.m_outlineColour = colour.GetAsNormalisedVec4();

        if (updateCB)
        {
            m_textCB.MapToGPU();
        }
    }

    void Text::SetTextHeight(float textHeight, bool updateCB)
    {
        m_textHeight = textHeight;

        if (updateCB)
        {
            UpdateScale();
            UpdatePositionOffset();

            m_textCB.MapToGPU();
        }
    }

    void Text::SetSpacingMod(float spacingMod, bool updateMesh)
    {
        m_spacingMod = spacingMod;

        if (updateMesh)
        {
            UpdateMesh();
        }
    }

    void Text::SetSpace(TextSpace textSpace)
    {
        m_textSpace = textSpace;
    }

    void Text::SetAnchorPosition(const Maths::Vec2f& anchorPosition, bool updateCB)
    {
        m_anchorPosition = anchorPosition;

        if (updateCB)
        {
            UpdatePositionOffset();

            m_textCB.MapToGPU();
        }
    }

    void Text::SetTextAnchor(TextAnchor textAnchor, bool updateCB)
    {
        m_textAnchor = textAnchor;

        if (updateCB)
        {
            UpdatePositionOffset();

            m_textCB.MapToGPU();
        }
    }

    void Text::UpdateMesh()
    {
        std::vector<UnlitVertex> vertices;
        std::vector<DWORD>       indices;

        m_font->RenderText(m_string, vertices, indices, m_textBaseExtents, m_spacingMod);

        if (m_mesh != nullptr)
        {
            delete m_mesh;
        }
        m_mesh = new Mesh(m_string, vertices, indices, m_font->GetTexture());

        UpdateScale();

        UpdatePositionOffset();

        MapCBToGPU();
    }

    void Text::UpdatePositionOffset()
    {
        const Maths::Vec2f& windowDimensions = Engine::Instance().GetWindowDimensionsFloat();

        auto getPositionOffsetForScreenSpaceCentrePos = [&](const Maths::Vec2f& screenCentrePos)
        {
            Maths::Vec2f offset = (screenCentrePos - (windowDimensions * 0.5f)) / (windowDimensions * 0.5f);
            offset.y = -offset.y;
            return offset;
        };

        switch (m_textAnchor)
        {
        case TextAnchor::e_TopLeft:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition + m_textBaseExtents * m_textHeight * 0.5f);
            break;
        case TextAnchor::e_TopMid:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition + Maths::Vec2f(0.0f, m_textBaseExtents.y * m_textHeight * 0.5f));
            break;
        case TextAnchor::e_TopRight:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition + Maths::Vec2f(-m_textBaseExtents.x * m_textHeight * 0.5f, m_textBaseExtents.y * m_textHeight * 0.5f));
            break;
        case TextAnchor::e_MidLeft:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition + Maths::Vec2f(m_textBaseExtents.x * m_textHeight * 0.5f, 0.0f));
            break;
        case TextAnchor::e_MidMid:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition);
            break;
        case TextAnchor::e_MidRight:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition + Maths::Vec2f(-m_textBaseExtents.x * m_textHeight * 0.5f, 0.0f));
            break;
        case TextAnchor::e_BotLeft:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition + Maths::Vec2f(m_textBaseExtents.x * m_textHeight * 0.5f, -m_textBaseExtents.y * m_textHeight * 0.5f));
            break;
        case TextAnchor::e_BotMid:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition + Maths::Vec2f(0.0f, -m_textBaseExtents.y * m_textHeight * 0.5f));
            break;
        case TextAnchor::e_BotRight:
            m_textCB.m_data.m_positionOffset = getPositionOffsetForScreenSpaceCentrePos(m_anchorPosition - m_textBaseExtents * m_textHeight * 0.5f);
            break;
        }
    }

    void Text::UpdateScale()
    {
        Maths::Vec2f textSizePixels = Maths::Vec2f(m_textBaseExtents.x * m_textHeight, m_textBaseExtents.y * m_textHeight);
        
        const Maths::Vec2f& windowDimensions = Engine::Instance().GetWindowDimensionsFloat();

        m_textCB.m_data.m_scale.x = textSizePixels.x / windowDimensions.x;
        m_textCB.m_data.m_scale.y = textSizePixels.y / windowDimensions.y;

        auto lerp = [&](float a, float b, float t)
        {
            return a + t * (b - a);
        };
        
        auto linearProgress = [&](float edge0, float edge1, float t)
        {
            return fminf(1.0f, fmaxf(0.0f, (t - edge0) / (edge1 - edge0)));
        };

        float heightNormalised = linearProgress(10.0f, 700.0f, m_textHeight);

        m_textCB.m_data.m_antiAliasDistanceRange = lerp(0.25f, 0.015f, 1.0f - powf(1.0f - heightNormalised, 5.0f));
    }

    void Text::MapCBToGPU() const
    {
        m_textCB.MapToGPU();
    }
}
