#pragma once

#include <string>

#include "Font.h"

#include "../../Maths/Vec2.h"

#include "../Data/Texture.h"
#include "../Data/Mesh.h"

namespace Firelight::Graphics
{
    class Text
    {
    public:
        Text(const std::string& string, const Maths::Vec2f& screenPos, Font* font);
        ~Text();

        const Mesh* GetMesh() const;

        void UpdateMesh();

    private:
        std::string  m_string;

        Maths::Vec2f m_screenPos;

        Mesh*        m_mesh;
        Font*        m_font;
    };
}
