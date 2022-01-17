#include "Material.h"

#include "Texture.h"

#include "../Shaders/VertexShader.h"
#include "../Shaders/PixelShader.h"

namespace Firelight::Graphics
{
    MaterialTexture::MaterialTexture() :
        m_bindSlot(-1),
        m_texture(nullptr)
    {
    }

    MaterialTexture::MaterialTexture(int bindSlot, Texture* texture) :
        m_bindSlot(bindSlot),
        m_texture(texture)
    {
    }

    Material::Material() :
        m_vertexShader(nullptr),
        m_pixelShader(nullptr),
        m_name("undefined")
    {
    }

    Material::~Material()
    {
    }

    bool Material::Initialise(const std::string& name)
    {
        // TODO: Make materials actually load data here

        return true;
    }

    void Material::Bind(bool bindPSData) const
    {
    }
}
