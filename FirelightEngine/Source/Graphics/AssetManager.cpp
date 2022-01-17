#include "AssetManager.h"

#include "Data/Texture.h"
#include "Data/Material.h"
#include "Data/Model.h"

#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"
#include "Shaders/ComputeShader.h"

#include "Data/VertexTypes.h"

namespace Firelight::Graphics
{
    AssetManager::AssetManager() :
        m_defaultTexture(nullptr),
        m_defaultMaterial(nullptr),
        m_defaultModel(nullptr),
        
        m_defaultVS(nullptr),
        m_defaultPS(nullptr),
        m_defaultCS(nullptr)
    {
    }

    AssetManager::~AssetManager()
    {
    }

    AssetManager& AssetManager::Instance()
    {
        static AssetManager instance;
        return instance;
    }

    bool AssetManager::Initialise()
    {
        m_defaultTexture = GetTexturePtr("$ENGINE/Textures/missing.png");
        m_defaultMaterial = GetMaterialPtr("$ENGINE/Materials/default");
        m_defaultModel = GetModelPtr<FancyLitVertex>("$ENGINE/Models/cube.obj");

        m_defaultVS = GetVSPtr<FancyLitVertex>("$ENGINE/Shaders/Vertex/Unlit");
        m_defaultPS = GetPSPtr("$ENGINE/Shaders/Pixel/Unlit");
        m_defaultCS = nullptr;

        return true;
    }

    Texture* AssetManager::GetTexturePtr(const std::string& path)
    {
        const auto& textureItr = m_textures.find(path);
        if (textureItr != m_textures.end())
        {
            return textureItr->second;
        }
        else
        {
            Texture* loadedTexture = new Texture();
            if (!loadedTexture->Initialise("Resources/" + path))
            {
                delete loadedTexture;
                loadedTexture = m_defaultTexture;
            }
            m_textures.insert({ path, loadedTexture });
            return loadedTexture;
        }
    }

    Texture* AssetManager::GetColourTexturePtr(Colour colour)
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

    Material* AssetManager::GetMaterialPtr(const std::string& path)
    {
        const auto& materialItr = m_materials.find(path);
        if (materialItr != m_materials.end())
        {
            return materialItr->second;
        }
        else
        {
            Material* newMaterial = new Material();
            if (!newMaterial->Initialise("Resources/" + path))
            {
                delete newMaterial;
                newMaterial = m_defaultMaterial;
            }
            m_materials.insert({ path, newMaterial });
            return newMaterial;
        }
    }

    PixelShader* AssetManager::GetPSPtr(const std::string& path)
    {
        const auto& pixelShaderItr = m_pixelShaders.find(path);
        if (pixelShaderItr != m_pixelShaders.end())
        {
            return pixelShaderItr->second;
        }
        else
        {
            PixelShader* newPixelShader = new PixelShader();
            std::string shaderPath = "Resources/" + path + ".hlsl";
            if (!newPixelShader->Initialise(shaderPath.c_str()))
            {
                delete newPixelShader;
                newPixelShader = m_defaultPS;
            }
            m_pixelShaders.insert({ path, newPixelShader });
            return newPixelShader;
        }
    }

    ComputeShader* AssetManager::GetCSPtr(const std::string& path)
    {
        const auto& computeShaderItr = m_computeShaders.find(path);
        if (computeShaderItr != m_computeShaders.end())
        {
            return computeShaderItr->second;
        }
        else
        {
            ComputeShader* newComputeShader = new ComputeShader();
            std::string shaderPath = "Resources/" + path + ".hlsl";
            if (!newComputeShader->Initialise(shaderPath.c_str()))
            {
                delete newComputeShader;
                newComputeShader = m_defaultCS;
            }
            m_computeShaders.insert({ path, newComputeShader });
            return newComputeShader;
        }
    }

    Texture* AssetManager::GetDefaultTexturePtr()
    {
        return m_defaultTexture;
    }

    Material* AssetManager::GetDefaultMaterialPtr()
    {
        return m_defaultMaterial;
    }

    Model* AssetManager::GetDefaultModelPtr()
    {
        return m_defaultModel;
    }

    VertexShader* AssetManager::GetDefaultVSPtr()
    {
        return m_defaultVS;
    }

    PixelShader* AssetManager::GetDefaultPSPtr()
    {
        return m_defaultPS;
    }

    ComputeShader* AssetManager::GetDefaultCSPtr()
    {
        return m_defaultCS;
    }
}
