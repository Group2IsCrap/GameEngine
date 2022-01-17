#pragma once

#include <unordered_map>
#include <string>

#include "Data/Colour.h"

// Forward declarations
namespace Firelight::Graphics
{
	class Texture;
	class Material;
	class Model;

	class VertexShader;
	class PixelShader;
	class ComputeShader;
}

namespace Firelight::Graphics
{
    class ResourceManager
    {
    public:
        ~ResourceManager();

        static ResourceManager& Instance();

		bool Initialise();

		// Standard resource getters
		Texture*       GetTexturePtr(const std::string& path);
		Texture*       GetTexturePtr(const std::string& path, const uint8_t* pData, size_t size);
		Texture*       GetColourTexturePtr(Colour colour);
		Material*      GetMaterialPtr(const std::string& name);
		template<typename VertexType>
		Model*         GetModelPtr(const std::string& path);

		template<typename VertexType>
		VertexShader*  GetVSPtr(const std::string& name);
		PixelShader*   GetPSPtr(const std::string& name);
		ComputeShader* GetCSPtr(const std::string& name);

		// Default resource getters
		Texture*       GetDefaultTexturePtr();
		Material*      GetDefaultMaterialPtr();
		Model*         GetDefaultModelPtr();

		VertexShader*  GetDefaultVSPtr();
		PixelShader*   GetDefaultPSPtr();
		ComputeShader* GetDefaultCSPtr();

    private:
        ResourceManager();

	private:
		std::unordered_map<std::string, Texture*>     m_textures;
		std::unordered_map<unsigned int, Texture*>    m_colourTextures;
		std::unordered_map<std::string, Material*>    m_materials;
		std::unordered_map<std::string, Model*>       m_models;

		std::unordered_map<std::string, VertexShader*>  m_vertexShaders;
		std::unordered_map<std::string, PixelShader*>   m_pixelShaders;
		std::unordered_map<std::string, ComputeShader*> m_computeShaders;

		Texture*  m_defaultTexture;
		Material* m_defaultMaterial;
		Model*    m_defaultModel;

		VertexShader*  m_defaultVS;
		PixelShader*   m_defaultPS;
		ComputeShader* m_defaultCS;
    };
}

#include "ResourceManager.inl"
