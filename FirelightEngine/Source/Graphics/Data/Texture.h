
#pragma once

#include <string>

#include <d3d11.h>
#include <wrl/client.h>

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "Colour.h"

namespace Firelight::Graphics
{
	class Texture
	{
	public:
		Texture();
		Texture(const Colour& colour);
		Texture(const Colour* colourData, UINT width, UINT height);
		Texture(const std::string& filePath);
		Texture(const uint8_t* pData, size_t size);

		bool Initialize(const std::string& filePath);

		Microsoft::WRL::ComPtr<ID3D11Resource>&           GetResource();
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView();

	private:
		void InitializeColourTexture(const Colour* colourData, UINT width, UINT height);
		void Initialize1x1ColourTexture(const Colour& colour);

	private:
		Microsoft::WRL::ComPtr<ID3D11Resource>           m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;
	};
}