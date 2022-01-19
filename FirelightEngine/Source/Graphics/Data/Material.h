#pragma once

#include <vector>
#include <string>

// Forward declarations
namespace Firelight::Graphics
{
    class Texture;
    class VertexShader;
    class PixelShader;
}

namespace Firelight::Graphics
{
    struct MaterialTexture
    {
        MaterialTexture();
        MaterialTexture(int bindSlot, Texture* texture);

        int      m_bindSlot;
        Texture* m_texture;
    };

    class Material
    {
    public:
        enum class TextureType
        {
            e_Albedo,
            e_Roughness,
            e_Normal,
            e_Metallic,
            e_Emission,
            e_Depth,
            e_NumTypes,
        };

    public:
        Material();
        ~Material();

        bool Initialise(const std::string& path);

        void Bind(bool bindPSData = true) const;

        Texture* GetDefaultTexture(TextureType textureType);

    private:
        void AssignDefaultTextures();

    private:
        std::vector<MaterialTexture> m_textures;

        VertexShader* m_vertexShader;
        PixelShader*  m_pixelShader;

        std::string   m_name;
    };
}
