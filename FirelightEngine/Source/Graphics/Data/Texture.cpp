#include "Texture.h"

#include <WICTextureLoader.h>
#include <DDSTextureLoader.h>

#include "../GraphicsHandler.h"

#include "../../Utils/ErrorManager.h"
#include "../../utils/StringHelpers.h"

namespace Firelight::Graphics
{
	Texture::Texture() :
		m_texture(nullptr),
		m_textureView(nullptr),

		m_dimensions(0)
	{
	}

	Texture::Texture(const Colour::RGBA& colour) :
		Texture()
	{
		Initialise1x1ColourTexture(colour);
	}

	Texture::Texture(const Colour::RGBA* colourData, UINT width, UINT height) :
		Texture()
	{
		InitialiseColourTexture(colourData, width, height);
	}

	Texture::Texture(const std::string& filePath) :
		Texture()
	{
		Initialise(filePath);
	}

	Texture::Texture(const uint8_t* pData, size_t size) :
		Texture()
	{
		HRESULT hr = DirectX::CreateWICTextureFromMemory(GraphicsHandler::Instance().GetDevice(), pData, size, m_texture.GetAddressOf(), m_textureView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create texture from memory");

		UpdateDimensionsUsingResource2D();
	}

	bool Texture::Initialise(const std::string& filePath)
	{
		try 
		{
			if (Utils::StringHelpers::GetFileExtension(filePath) == "dds")
			{
				HRESULT hr = DirectX::CreateDDSTextureFromFile(GraphicsHandler::Instance().GetDevice(), Utils::StringHelpers::StringToWide(filePath).c_str(), m_texture.GetAddressOf(), m_textureView.GetAddressOf());
				COM_ERROR_THROW_IF_FAILED(hr, "Failed to load texture at path: " + filePath);
			}
			else
			{
				HRESULT hr = DirectX::CreateWICTextureFromFile(GraphicsHandler::Instance().GetDevice(), Utils::StringHelpers::StringToWide(filePath).c_str(), m_texture.GetAddressOf(), m_textureView.GetAddressOf());
				COM_ERROR_THROW_IF_FAILED(hr, "Failed to load texture at path: " + filePath);
			}

			UpdateDimensionsUsingResource2D();
		}
		catch (COMError& comError)
		{
			(void)comError;
			Initialise1x1ColourTexture(Colours::sc_unhandledTextureColour);
			return false;
		}
		
		return true;
	}

	Microsoft::WRL::ComPtr<ID3D11Resource>& Texture::GetResource()
	{
		return m_texture;
	}

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& Texture::GetShaderResourceView()
	{
		return m_textureView;
	}

	const Maths::Vec3i& Texture::GetDimensions() const
	{
		return m_dimensions;
	}

	Maths::Vec2f Texture::GetTexCoordFromSourcePixelCoord(const Maths::Vec2f& sourcePixelCoord) const
	{
		Maths::Vec2f returnVec;

		returnVec.x = sourcePixelCoord.x / m_dimensions.x;
		returnVec.y = sourcePixelCoord.y / m_dimensions.y;

		return returnVec;
	}

	void Texture::InitialiseColourTexture(const Colour::RGBA* colourData, UINT width, UINT height)
	{
		m_dimensions.x = width;
		m_dimensions.y = height;

		CD3D11_TEXTURE2D_DESC textureDesc(DXGI_FORMAT_R8G8B8A8_UNORM, width, height);

		ID3D11Texture2D* p2DTexture = nullptr;

		D3D11_SUBRESOURCE_DATA initialData{};
		initialData.pSysMem = colourData;
		initialData.SysMemPitch = width * sizeof(Colour::RGBA);

		HRESULT hr = GraphicsHandler::Instance().GetDevice()->CreateTexture2D(&textureDesc, &initialData, &p2DTexture);

		COM_ERROR_IF_FAILED(hr, "Failed to initialize texture from colour data");

		m_texture = p2DTexture;

		CD3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc(D3D11_SRV_DIMENSION_TEXTURE2D, textureDesc.Format);
		hr = GraphicsHandler::Instance().GetDevice()->CreateShaderResourceView(m_texture.Get(), &shaderResourceViewDesc, m_textureView.GetAddressOf());
		COM_ERROR_IF_FAILED(hr, "Failed to create shader resource view for colour texture");
	}

	void Texture::Initialise1x1ColourTexture(const Colour::RGBA& colour)
	{
		InitialiseColourTexture(&colour, 1, 1);
	}

	void Texture::UpdateDimensionsUsingResource2D()
	{
		if (m_texture.Get() != nullptr)
		{
			ID3D11Texture2D* pTextureInterface = nullptr;
			m_texture->QueryInterface<ID3D11Texture2D>(&pTextureInterface);

			ASSERT_IF(pTextureInterface != nullptr, "Failed to create texture dimensions, could not query interface for texture");
			{
				D3D11_TEXTURE2D_DESC desc;
				pTextureInterface->GetDesc(&desc);

				m_dimensions.x = desc.Width;
				m_dimensions.y = desc.Height;
			}
		}
	}
}
