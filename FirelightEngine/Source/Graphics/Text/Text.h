#pragma once

#include <string>

#include "Font.h"

#include "../../Maths/Vec2.h"

#include "../Data/Texture.h"
#include "../Data/Mesh.h"

#include "../Buffers/ConstantBuffer.h"
#include "../Buffers/ConstantBufferTypes.h"

namespace Firelight::Graphics
{
    enum class TextSpace
    {
        e_World,
        e_Pixel
    };

    enum class TextAnchor
    {
        
        e_TopLeft,
        e_TopMid,
        e_TopRight,
        e_MidLeft,
        e_MidMid,
        e_MidRight,
        e_BotLeft,
        e_BotMid,
        e_BotRight
    };

    enum class TextAlign
    {
        e_Centre,
        e_Left,
        e_Right
    };

    class Text
    {
    public:
        Text(const std::string& string, Font* font);
        ~Text();

        void Draw() const;

        const Mesh* GetMesh() const;

        void SetString(const std::string& string, bool updateMesh = true);
        void SetFont(Font* font, bool updateMesh = true);

        void SetColour(const Colour::RGBA& colour, bool updateCB = true);

        void SetOutlineEnabled(bool enabled, bool updateCB = true);
        void SetOutlineWidth(float width, bool updateCB = true);
        void SetOutlineColour(const Colour::RGBA& colour, bool updateCB = true);

        void SetTextHeight(float textHeight, bool updateCB = true);
        void SetSpacingMod(float spacingMod, bool updateMesh = true);
        void SetAnchorPosition(const Maths::Vec2f& position, bool updateCB = true);
        void SetSpace(TextSpace textSpace);

        void UpdateMesh();
        void UpdatePositionOffset();
        void UpdateScale();

        void MapCBToGPU() const;

    private:
        std::string  m_string;
        Font*        m_font;

        Maths::Vec2f m_anchorPosition;
        TextSpace    m_textSpace;
        TextAnchor   m_textAnchor;
        TextAlign    m_textAlign;

        float        m_textHeight;
        float        m_spacingMod;
        float        m_maxLineWidth;

        Maths::Vec2f           m_textBaseExtents;
        Mesh*                  m_mesh;
        ConstantBuffer<TextCB> m_textCB;
    };
}
