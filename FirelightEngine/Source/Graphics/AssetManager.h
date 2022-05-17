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

	class Font;
}

namespace Firelight::Graphics
{
    class AssetManager
    {
    public:
        ~AssetManager();

        static AssetManager& Instance();

		bool Initialise();

		// Standard resource getters
		Texture*       GetTexture(const std::string& path);
		Texture*       GetColourTexture(const Colour::RGBA& colour);
		Material*      GetMaterial(const std::string& path);
		template<typename VertexType>
		Model*         GetModel(const std::string& path);

		VertexShader*  GetVertexShader(const std::string& path);
		PixelShader*   GetPixelShader(const std::string& path);
		ComputeShader* GetComputeShader(const std::string& path);

		Font*          GetFont(const std::string& path);

		// Default resource getters
		Texture*       GetDefaultTexture();
		Material*      GetDefaultMaterial();
		Model*         GetDefaultModel();

		VertexShader*  GetDefaultVertexShader();
		PixelShader*   GetDefaultPixelShader();
		ComputeShader* GetDefaultComputeShader();

		Font*          GetDefaultFont();

		// Other getters
		Model*         GetScreenQuad();

		Material*      GetCurrentlyBoundMaterial();

    private:
		AssetManager();

	private:
		// All assets
		std::unordered_map<std::string, Texture*>  m_textures;
		std::unordered_map<unsigned int, Texture*> m_colourTextures;
		std::unordered_map<std::string, Material*> m_materials;
		std::unordered_map<std::string, Model*>    m_models;

		std::unordered_map<std::string, VertexShader*>  m_vertexShaders;
		std::unordered_map<std::string, PixelShader*>   m_pixelShaders;
		std::unordered_map<std::string, ComputeShader*> m_computeShaders;

		std::unordered_map<std::string, Font*> m_fonts;

		// Default assets
		Texture*  m_defaultTexture;
		Material* m_defaultMaterial;
		Model*    m_defaultModel;

		VertexShader*  m_defaultVS;
		PixelShader*   m_defaultPS;
		ComputeShader* m_defaultCS;

		Font* m_defaultFont;

		// Other
		Material* m_currentlyBoundMaterial;

		Model* m_screenQuad;
    };
}

#include "AssetManager.inl"
