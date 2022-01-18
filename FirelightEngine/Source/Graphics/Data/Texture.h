
#pragma once

#include <string>

#include <d3d11.h>
#include <wrl/client.h>

#include "Colour.h"

#include "../../Maths/Vec3.h"

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

		bool Initialise(const std::string& filePath);

		Microsoft::WRL::ComPtr<ID3D11Resource>&           GetResource();
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView();

		const Maths::Vec3i& GetDimensions() const;
		Maths::Vec2f GetTexCoordFromSourcePixelCoord(const Maths::Vec2f& sourcePixelCoord);

	private:
		void InitialiseColourTexture(const Colour* colourData, UINT width, UINT height);
		void Initialise1x1ColourTexture(const Colour& colour);

		void UpdateDimensionsUsingResource2D();

	private:
		Microsoft::WRL::ComPtr<ID3D11Resource>           m_texture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_textureView;

		Maths::Vec3i                                     m_dimensions;
	};
}