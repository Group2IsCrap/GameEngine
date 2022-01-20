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
        m_defaultTexture = GetTexture("$ENGINE/Textures/missing.png");
        m_defaultMaterial = GetMaterial("$ENGINE/Materials/default");
        m_defaultModel = GetModel<FancyLitVertex>("$ENGINE/Models/cube.obj");

        m_defaultVS = GetVertexShader("$ENGINE/Shaders/Vertex/Unlit");
        m_defaultPS = GetPixelShader("$ENGINE/Shaders/Pixel/Unlit");
        m_defaultCS = nullptr;

        m_screenQuad = GetModel<UnlitVertex>("$ENGINE/Models/screenQuad.obj");

        return true;
    }

    Texture* AssetManager::GetTexture(const std::string& path)
    {
        const auto& textureItr = m_textures.find(path);
        if (textureItr != m_textures.end())
        {
            return textureItr->second;
        }
        else
        {
            Texture* loadedTexture = new Texture();
            if (!loadedTexture->Initialise("Assets/" + path))
            {
                delete loadedTexture;
                loadedTexture = m_defaultTexture;
            }
            m_textures.insert({ path, loadedTexture });
            return loadedTexture;
        }
    }

    Texture* AssetManager::GetColourTexture(const Colour::RGBA& colour)
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

    Material* AssetManager::GetMaterial(const std::string& path)
    {
        const auto& materialItr = m_materials.find(path);
        if (materialItr != m_materials.end())
        {
            return materialItr->second;
        }
        else
        {
            Material* newMaterial = new Material();
            if (!newMaterial->Initialise("Assets/" + path))
            {
                delete newMaterial;
                newMaterial = m_defaultMaterial;
            }
            m_materials.insert({ path, newMaterial });
            return newMaterial;
        }
    }

    VertexShader* AssetManager::GetVertexShader(const std::string& path)
    {
        const auto& vertexShaderItr = m_vertexShaders.find(path);
        if (vertexShaderItr != m_vertexShaders.end())
        {
            return vertexShaderItr->second;
        }
        else
        {
            VertexShader* newVertexShader = new VertexShader();
            std::string shaderPath = "Assets/" + path + ".hlsl";
            if (!newVertexShader->Initialise(shaderPath.c_str()))
            {
                delete newVertexShader;
                newVertexShader = m_defaultVS;
            }
            m_vertexShaders.insert({ path, newVertexShader });
            return newVertexShader;
        }
    }

    PixelShader* AssetManager::GetPixelShader(const std::string& path)
    {
        const auto& pixelShaderItr = m_pixelShaders.find(path);
        if (pixelShaderItr != m_pixelShaders.end())
        {
            return pixelShaderItr->second;
        }
        else
        {
            PixelShader* newPixelShader = new PixelShader();
            std::string shaderPath = "Assets/" + path + ".hlsl";
            if (!newPixelShader->Initialise(shaderPath.c_str()))
            {
                delete newPixelShader;
                newPixelShader = m_defaultPS;
            }
            m_pixelShaders.insert({ path, newPixelShader });
            return newPixelShader;
        }
    }

    ComputeShader* AssetManager::GetComputeShader(const std::string& path)
    {
        const auto& computeShaderItr = m_computeShaders.find(path);
        if (computeShaderItr != m_computeShaders.end())
        {
            return computeShaderItr->second;
        }
        else
        {
            ComputeShader* newComputeShader = new ComputeShader();
            std::string shaderPath = "Assets/" + path + ".hlsl";
            if (!newComputeShader->Initialise(shaderPath.c_str()))
            {
                delete newComputeShader;
                newComputeShader = m_defaultCS;
            }
            m_computeShaders.insert({ path, newComputeShader });
            return newComputeShader;
        }
    }

    Texture* AssetManager::GetDefaultTexture()
    {
        return m_defaultTexture;
    }

    Material* AssetManager::GetDefaultMaterial()
    {
        return m_defaultMaterial;
    }

    Model* AssetManager::GetDefaultModel()
    {
        return m_defaultModel;
    }

    VertexShader* AssetManager::GetDefaultVertexShader()
    {
        return m_defaultVS;
    }

    PixelShader* AssetManager::GetDefaultPixelShader()
    {
        return m_defaultPS;
    }

    ComputeShader* AssetManager::GetDefaultComputeShader()
    {
        return m_defaultCS;
    }

    Model* AssetManager::GetScreenQuad()
    {
        return m_screenQuad;
    }
}
