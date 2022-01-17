
#include "ResourceManager.h"

#include "Data/Model.h"

#include "Shaders/VertexShader.h"

#include "../Utils/StringHelpers.h"

namespace Firelight::Graphics
{
    template<typename VertexType>
    Model* ResourceManager::GetModelPtr(const std::string& path)
    {
        const auto& modelItr = m_models.find(path);
        if (modelItr != m_models.end())
        {
            return modelItr->second;
        }
        else
        {
            Model* loadedModel = new Model();
            if (!loadedModel->Initialise<VertexType>(path))
            {
                delete loadedModel;
                loadedModel = m_defaultModel;
            }
            m_models.insert({ path, loadedModel });
            return loadedModel;
        }
    }

    template<typename VertexType>
    VertexShader* ResourceManager::GetVSPtr(const std::string& name)
    {
        const auto& vertexShaderItr = m_vertexShaders.find(name);
        if (vertexShaderItr != m_vertexShaders.end())
        {
            return vertexShaderItr->second;
        }
        else
        {
            VertexShader* newVertexShader = new VertexShader();
            std::string shaderPath = GraphicsHandler::Instance().GetCompiledShaderFolder() + "vs_" + name + ".cso";
            if (!newVertexShader->Initialise(shaderPath.c_str(), &VertexType::sc_inputElements[0], VertexType::sc_inputElementCount))
            {
                delete newVertexShader;
                newVertexShader = m_defaultVS;
            }
            m_vertexShaders.insert({ name, newVertexShader });
            return newVertexShader;
        }
    }
}