#include "ResourceManager.h"

#include "Data/Texture.h"
#include "Data/Material.h"
#include "Data/Model.h"

#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include "Shaders/ComputeShader.h"

#include "Data/VertexTypes.h"

namespace Firelight::Graphics
{
    ResourceManager::ResourceManager() :
        m_defaultTexture(nullptr),
        m_defaultMaterial(nullptr),
        m_defaultModel(nullptr),
        
        m_defaultVS(nullptr),
        m_defaultPS(nullptr),
        m_defaultCS(nullptr)
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    ResourceManager& ResourceManager::Instance()
    {
        static ResourceManager instance;
        return instance;
    }

    bool ResourceManager::Initialise()
    {
        m_defaultTexture = GetTexturePtr("Resources/Engine/Textures/missing.png");
        m_defaultMaterial = GetMaterialPtr("Resources/Engine/Materials/default");
        m_defaultModel = GetModelPtr<FancyLitVertex>("Resources/Engine/Models/cube.obj");

        m_defaultVS = GetVSPtr<FancyLitVertex>("default");
        m_defaultPS = GetPSPtr("default");
        m_defaultCS = nullptr;

        return true;
    }

    Texture* ResourceManager::GetTexturePtr(const std::string& path)
    {
        const auto& textureItr = m_textures.find(path);
        if (textureItr != m_textures.end())
        {
            return textureItr->second;
        }
        else
        {
            Texture* loadedTexture = new Texture();
            if (!loadedTexture->Initialise(path))
            {
                delete loadedTexture;
                loadedTexture = m_defaultTexture;
            }
            m_textures.insert({ path, loadedTexture });
            return loadedTexture;
        }
    }

    Texture* ResourceManager::GetTexturePtr(const std::string& path, const uint8_t* pData, size_t size)
    {
        const auto& textureItr = m_textures.find(path);
        if (textureItr != m_textures.end())
        {
            return textureItr->second;
        }
        else
        {
            Texture* newTexture = new Texture(pData, size);
            if (!newTexture->Initialise(path))
            {
                delete newTexture;
                newTexture = m_defaultTexture;
            }
            m_textures.insert({ path, newTexture });
            return newTexture;
        }
    }

    Texture* ResourceManager::GetColourTexturePtr(Colour colour)
    {
        const auto& textureItr = m_colourTextures.find(colour.GetUnsignedInt());
        if (textureItr != m_colourTextures.end())
        {
            return textureItr->second;
        }
        else
        {
            Texture* newTexture = new Texture(colour);
            m_colourTextures.insert({ colour.GetUnsignedInt(), newTexture });
            return newTexture;
        }
    }

    Material* ResourceManager::GetMaterialPtr(const std::string& name)
    {
        const auto& materialItr = m_materials.find(name);
        if (materialItr != m_materials.end())
        {
            return materialItr->second;
        }
        else
        {
            Material* newMaterial = new Material();
            if (!newMaterial->Initialise(name))
            {
                delete newMaterial;
                newMaterial = m_defaultMaterial;
            }
            m_materials.insert({ name, newMaterial });
            return newMaterial;
        }
    }

    PixelShader* ResourceManager::GetPSPtr(const std::string& name)
    {
        const auto& pixelShaderItr = m_pixelShaders.find(name);
        if (pixelShaderItr != m_pixelShaders.end())
        {
            return pixelShaderItr->second;
        }
        else
        {
            PixelShader* newPixelShader = new PixelShader();
            std::string shaderPath = GraphicsHandler::Instance().GetCompiledShaderFolder() + "ps_" + name + ".cso";
            if (!newPixelShader->Initialise(shaderPath.c_str()))
            {
                delete newPixelShader;
                newPixelShader = m_defaultPS;
            }
            m_pixelShaders.insert({ name, newPixelShader });
            return newPixelShader;
        }
    }

    ComputeShader* ResourceManager::GetCSPtr(const std::string& name)
    {
        const auto& computeShaderItr = m_computeShaders.find(name);
        if (computeShaderItr != m_computeShaders.end())
        {
            return computeShaderItr->second;
        }
        else
        {
            ComputeShader* newComputeShader = new ComputeShader();
            std::string shaderPath = GraphicsHandler::Instance().GetCompiledShaderFolder() + "cs_" + name + ".cso";
            if (!newComputeShader->Initialise(shaderPath.c_str()))
            {
                delete newComputeShader;
                newComputeShader = m_defaultCS;
            }
            m_computeShaders.insert({ name, newComputeShader });
            return newComputeShader;
        }
    }

    Texture* ResourceManager::GetDefaultTexturePtr()
    {
        return m_defaultTexture;
    }

    Material* ResourceManager::GetDefaultMaterialPtr()
    {
        return m_defaultMaterial;
    }

    Model* ResourceManager::GetDefaultModelPtr()
    {
        return m_defaultModel;
    }

    VertexShader* ResourceManager::GetDefaultVSPtr()
    {
        return m_defaultVS;
    }

    PixelShader* ResourceManager::GetDefaultPSPtr()
    {
        return m_defaultPS;
    }

    ComputeShader* ResourceManager::GetDefaultCSPtr()
    {
        return m_defaultCS;
    }
}
