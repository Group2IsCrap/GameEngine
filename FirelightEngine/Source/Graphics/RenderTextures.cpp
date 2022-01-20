#include "RenderTextures.h"

#include "GraphicsHandler.h"

#include "AssetManager.h"
#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"


#include "../utils/ErrorManager.h"

namespace Firelight::Graphics
{
	IRenderTexture::IRenderTexture() :
		m_format(DXGI_FORMAT_UNKNOWN),
		m_width(0),
		m_height(0)
	{
	}

	bool IRenderTexture::Initialise(DXGI_FORMAT format, UINT width, UINT height, bool isDepthTex)
	{
		m_format = format;
		m_width = width;
		m_height = height;
		m_viewport = CD3D11_VIEWPORT(0.0f, 0.0f, (float)width, (float)height);

		// Create Texture2D description
		D3D11_TEXTURE2D_DESC textureDesc{};
		textureDesc.Width = width;
		textureDesc.Height = height;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.SampleDesc.Quality = 0;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;
		textureDesc.Format = m_format;

		if (isDepthTex)
		{
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL;
			textureDesc.Format = DXGI_FORMAT_R32_TYPELESS;
		}
		else
		{
			textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		}

		// Create Texture2D
		HRESULT hr = GraphicsHandler::Instance().GetDevice()->CreateTexture2D(&textureDesc, nullptr, m_texture2D.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "IRenderTexture Texture2D creation failed", false);

		// Create shader resource view description
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = m_format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;

		// Create shader resource view
		hr = GraphicsHandler::Instance().GetDevice()->CreateShaderResourceView(m_texture2D.Get(), &shaderResourceViewDesc, m_shaderResourceView.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "IRenderTexture shader resource view creation failed", false);

		return true;
	}

	void IRenderTexture::Release()
	{
		m_format = DXGI_FORMAT_UNKNOWN;

		m_texture2D.Reset();
		m_shaderResourceView.Reset();
	}

	bool IRenderTexture::CanDirectCopyTo(IRenderTexture* renderTexture)
	{
		bool cantDirectCopyTo = false;
		cantDirectCopyTo |= m_format != renderTexture->m_format;
		cantDirectCopyTo |= m_width != renderTexture->m_width;
		cantDirectCopyTo |= m_height != renderTexture->m_height;
		return !cantDirectCopyTo;
	}

	RenderTexture::RenderTexture() :
		IRenderTexture()
	{
	}

	bool RenderTexture::Initialise(DXGI_FORMAT format, UINT width, UINT height)
	{
		if (!IRenderTexture::Initialise(format, width, height, false))
		{
			return false;
		}

		// Create render target view description
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc{};
		renderTargetViewDesc.Format = m_format;
		renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

		// Create render target view
		HRESULT hr = GraphicsHandler::Instance().GetDevice()->CreateRenderTargetView(m_texture2D.Get(), &renderTargetViewDesc, m_renderTargetView.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "RenderTexture render target view creation failed", false);

		return true;
	}

	void RenderTexture::Release()
	{
		IRenderTexture::Release();

		m_renderTargetView.Reset();
	}

	void RenderTexture::CopyTo(RenderTexture* destination)
	{
		ID3D11DeviceContext* deviceContext = GraphicsHandler::Instance().GetDeviceContext();
		if (CanDirectCopyTo(destination))
		{
			GraphicsHandler::Instance().GetDeviceContext()->CopyResource(destination->m_texture2D.Get(), m_texture2D.Get());
		}
		else
		{
			// Set self as shader resource
			deviceContext->PSSetShaderResources(0, 1, m_shaderResourceView.GetAddressOf());

			deviceContext->PSSetShader(AssetManager::Instance().GetPixelShader("$ENGINE/Shaders/Pixel/QuadCopy")->GetShader(), NULL, 0);

			destination->SetAsRenderTargetAndDrawQuad();

			// Unset self as shader resource
			ID3D11ShaderResourceView* const nullShaderResourceViews[1] = { NULL };
			deviceContext->PSSetShaderResources(0, 1, nullShaderResourceViews);
		}
	}

	void RenderTexture::SetAsRenderTargetAndDrawQuad(bool clearRenderTarget)
	{
		ID3D11DeviceContext* deviceContext = GraphicsHandler::Instance().GetDeviceContext();

		float blackColour[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

		deviceContext->RSSetViewports(1, &m_viewport);

		deviceContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), nullptr);

		if (clearRenderTarget) deviceContext->ClearRenderTargetView(m_renderTargetView.Get(), blackColour);

		deviceContext->IASetInputLayout(AssetManager::Instance().GetVertexShader("$ENGINE/Shaders/Vertex/Quad")->GetInputLayout());
		deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		AssetManager::Instance().GetScreenQuad()->DrawRaw(false);

		// Unset self as render target view
		ID3D11RenderTargetView* const nullRenderTargetViews[1] = { NULL };
		deviceContext->OMSetRenderTargets(1, nullRenderTargetViews, nullptr);
	}

	DepthRenderTexture::DepthRenderTexture() :
		IRenderTexture()
	{
	}

	bool DepthRenderTexture::Initialise(DXGI_FORMAT format, UINT width, UINT height)
	{
		if (!IRenderTexture::Initialise(format, width, height, true))
		{
			return false;
		}

		// Create depth stencil view description
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		depthStencilViewDesc.Flags = 0;
		depthStencilViewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		depthStencilViewDesc.Texture2D.MipSlice = 0;

		// Create depth stencil view
		HRESULT hr = GraphicsHandler::Instance().GetDevice()->CreateDepthStencilView(m_texture2D.Get(), &depthStencilViewDesc, m_depthStencilView.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "DepthTexture depth stencil view creation failed", false);

		return true;
	}

	void DepthRenderTexture::Release()
	{
		IRenderTexture::Release();

		m_depthStencilView.Reset();
	}
}
