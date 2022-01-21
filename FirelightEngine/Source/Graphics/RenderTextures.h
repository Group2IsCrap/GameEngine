#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace Firelight::Graphics
{
	struct IRenderTexture
	{
	protected:
		IRenderTexture();

		bool Initialise(DXGI_FORMAT format, UINT width, UINT height, bool isDepthTex);
		void Release();

	public:
		bool CanDirectCopyTo(IRenderTexture* renderTexture);

	public:
		DXGI_FORMAT     m_format;
		UINT            m_width;
		UINT            m_height;
		CD3D11_VIEWPORT m_viewport;

		Microsoft::WRL::ComPtr<ID3D11Texture2D>          m_texture2D = nullptr;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView = nullptr;
	};

	struct RenderTexture : public IRenderTexture
	{
		RenderTexture();

		bool Initialise(DXGI_FORMAT format, UINT width, UINT height);
		void Release();

		void CopyTo(RenderTexture* destination);
		void SetAsRenderTargetAndDrawQuad(bool clearRenderTarget = true);

		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView = nullptr;
	};

	struct DepthRenderTexture : public IRenderTexture
	{
		DepthRenderTexture();

		bool Initialise(DXGI_FORMAT format, UINT width, UINT height);
		void Release();

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView = nullptr;
	};
}
