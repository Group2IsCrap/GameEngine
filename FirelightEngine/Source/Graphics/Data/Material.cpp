#include "Material.h"

#include "Texture.h"

#include "../GraphicsHandler.h"
#include "../AssetManager.h"

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

    bool Material::Initialise(const std::string& path)
    {
        // TODO: Make materials actually load data here,
        // for now lets just set to use default textures
        (void)path;

        AssignDefaultTextures();

        return true;
    }

    void Material::Bind(bool bindPSData) const
    {
        ID3D11DeviceContext* deviceContext = GraphicsHandler::Instance().GetDeviceContext();

        deviceContext->VSSetShader(m_vertexShader->GetShader(), nullptr, 0);

        if (bindPSData)
        {
            deviceContext->PSSetShader(m_pixelShader->GetShader(), nullptr, 0);

            for (int textureIndex = 0; textureIndex < m_textures.size(); ++textureIndex)
            {
                const MaterialTexture& materialTexture = m_textures[textureIndex];
                deviceContext->PSSetShaderResources(materialTexture.m_bindSlot, 1, materialTexture.m_texture->GetShaderResourceView().GetAddressOf());
            }
        }
    }

    Texture* Material::GetDefaultTexture(TextureType textureType)
    {
        switch (textureType)
        {
        case TextureType::e_Albedo:
            return AssetManager::Instance().GetColourTexture(Colours::sc_defaultAlbedo);
        case TextureType::e_Roughness:
            return AssetManager::Instance().GetColourTexture(Colours::sc_defaultRoughness);
        case TextureType::e_Normal:
            return AssetManager::Instance().GetColourTexture(Colours::sc_defaultNormal);
        case TextureType::e_Metallic:
            return AssetManager::Instance().GetColourTexture(Colours::sc_defaultMetallic);
        case TextureType::e_Emission:
            return AssetManager::Instance().GetColourTexture(Colours::sc_defaultEmission);
        case TextureType::e_Depth:
            return AssetManager::Instance().GetColourTexture(Colours::sc_defaultDepth);
        case TextureType::e_NumTypes:
            return nullptr;
        }
        return nullptr;
    }

    void Material::AssignDefaultTextures()
    {
        for (int textureSlot = 0; textureSlot < static_cast<int>(TextureType::e_NumTypes); ++textureSlot)
        {
            bool needsDefault = true;
            for (int textureIndex = 0; textureIndex < m_textures.size(); ++textureIndex)
            {
                // If texture slot already been set then dont override with default
                if (m_textures[textureIndex].m_bindSlot == textureSlot)
                {
                    needsDefault = false;
                    break;
                }
            }

            if (needsDefault)
            {
                m_textures.push_back(MaterialTexture(textureSlot, GetDefaultTexture(static_cast<TextureType>(textureSlot))));
            }
        }
    }
}
