#include "Text.h"

#include <vector>

#include "../Data/VertexTypes.h"

namespace Firelight::Graphics
{
    Firelight::Graphics::Text::Text::Text(const std::string& string, const Maths::Vec2f& screenPos, Font* font) :
        m_string(string),
        m_screenPos(screenPos),
        m_mesh(nullptr),
        m_font(font)
    {
        UpdateMesh();
    }

    Firelight::Graphics::Text::Text::~Text()
    {
    }

    const Mesh* Firelight::Graphics::Text::Text::GetMesh() const
    {
        return m_mesh;
    }

    void Text::UpdateMesh()
    {
        std::vector<UnlitVertex> vertices;
        std::vector<DWORD>       indices;

        float widthHeightRatio = 0.0f;
        m_font->RenderText(m_string, m_screenPos, vertices, indices, widthHeightRatio, 1.0f);

        if (m_mesh != nullptr)
        {
            delete m_mesh;
        }
        m_mesh = new Mesh(vertices, indices, m_font->GetTexture());
    }
}
