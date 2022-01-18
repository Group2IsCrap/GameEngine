
#include "AssetManager.h"

#include "Data/Model.h"

namespace Firelight::Graphics
{
    template<typename VertexType>
    Model* AssetManager::GetModel(const std::string& path)
    {
        const auto& modelItr = m_models.find(path);
        if (modelItr != m_models.end())
        {
            return modelItr->second;
        }
        else
        {
            Model* loadedModel = new Model();
            if (!loadedModel->Initialise<VertexType>("Assets/" + path))
            {
                delete loadedModel;
                loadedModel = m_defaultModel;
            }
            m_models.insert({ path, loadedModel });
            return loadedModel;
        }
    }
}