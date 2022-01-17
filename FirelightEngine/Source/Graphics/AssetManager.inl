
#include "AssetManager.h"

#include "Data/Model.h"
#include "Shaders/VertexShader.h"

#include "../Utils/StringHelpers.h"

namespace Firelight::Graphics
{
    template<typename VertexType>
    Model* AssetManager::GetModelPtr(const std::string& path)
    {
        const auto& modelItr = m_models.find(path);
        if (modelItr != m_models.end())
        {
            return modelItr->second;
        }
        else
        {
            Model* loadedModel = new Model();
            if (!loadedModel->Initialise<VertexType>("Resources/" + path))
            {
                delete loadedModel;
                loadedModel = m_defaultModel;
            }
            m_models.insert({ path, loadedModel });
            return loadedModel;
        }
    }

    template<typename VertexType>
    VertexShader* AssetManager::GetVSPtr(const std::string& path)
    {
        const auto& vertexShaderItr = m_vertexShaders.find(path);
        if (vertexShaderItr != m_vertexShaders.end())
        {
            return vertexShaderItr->second;
        }
        else
        {
            VertexShader* newVertexShader = new VertexShader();
            std::string shaderPath = "Resources/" + path + ".hlsl";
            if (!newVertexShader->Initialise(shaderPath.c_str(), &VertexType::sc_inputElements[0], VertexType::sc_inputElementCount))
            {
                delete newVertexShader;
                newVertexShader = m_defaultVS;
            }
            m_vertexShaders.insert({ path, newVertexShader });
            return newVertexShader;
        }
    }
}