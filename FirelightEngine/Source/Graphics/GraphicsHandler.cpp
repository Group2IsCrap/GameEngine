#include "GraphicsHandler.h"

#include "AssetManager.h"
#include "SpriteBatch.h"

#include "../Utils/ErrorManager.h"
#include "../Utils/AdapterReader.h"
#include "../ImGuiUI/ImGuiManager.h"

#include "Shaders/VertexShader.h"
#include "Shaders/PixelShader.h"

#include "Text/Font.h"
#include "Text/Text.h"

namespace Firelight::Graphics
{
    GraphicsHandler::GraphicsHandler() :
		m_initialised(false),
		m_deviceInitialised(false)
    {
    }

    GraphicsHandler::~GraphicsHandler()
    {
		
    }

    GraphicsHandler& GraphicsHandler::Instance()
    {
        static GraphicsHandler instance;
        return instance;
    }

	bool GraphicsHandler::IsInitialised() const
	{
		return m_initialised;
	}

    bool GraphicsHandler::Initialize(HWND hwnd, const Maths::Vec2i& dimensions)
    {
        ASSERT_RETURN(!m_initialised, "GraphicsHandler was aleady initialised", false);
		ASSERT_RETURN(!m_deviceInitialised, "GraphicsHandler device was aleady initialised", false);

        bool result = InitialiseDirectX(hwnd, dimensions);
        ASSERT_RETURN(result, "DirectX initialisation failed", false);

		result = Firelight::ImGuiUI::ImGuiManager::Instance()->Initialise(hwnd, m_device.Get(), m_deviceContext.Get());
		ASSERT_RETURN(result, "ImGui initialisation failed", false);

		m_spriteBatch = std::make_unique<SpriteBatch>();
		m_spriteBatch->SetSortMode(SpriteBatch::SortMode::e_BackToFrontTexture);

		m_initialised = true;

        return true;
    }

    bool GraphicsHandler::InitialiseDirectX(HWND hwnd, const Maths::Vec2i& dimensions)
    {
		// Get graphics card adapters
		std::vector<Utils::AdapterData> adapters = Utils::AdapterReader::Instance().GetAdapters();
		ASSERT_RETURN(adapters.size() >= 1, "No DXGI Adapters Found", false);

		// Create device and swapchain
		DXGI_SWAP_CHAIN_DESC swapChainDescription;
		ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDescription.BufferDesc.Width = dimensions.x;
		swapChainDescription.BufferDesc.Height = dimensions.y;
		swapChainDescription.BufferDesc.RefreshRate.Numerator = 144; // VSync fps
		swapChainDescription.BufferDesc.RefreshRate.Denominator = 1;

		swapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDescription.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDescription.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDescription.SampleDesc.Count = 1;   // No anti aliasing
		swapChainDescription.SampleDesc.Quality = 0; // Lowest image quality

		swapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDescription.BufferCount = 1;

		swapChainDescription.OutputWindow = hwnd;
		swapChainDescription.Windowed = TRUE;

		swapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		swapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		HRESULT hr = D3D11CreateDeviceAndSwapChain(adapters[0].m_adapter, D3D_DRIVER_TYPE_UNKNOWN,
			NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, &swapChainDescription,
			m_swapChain.GetAddressOf(), m_device.GetAddressOf(),
			NULL, m_deviceContext.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create device and swapchain.", false);

		m_deviceInitialised = true;

		// Get backbuffer
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(m_backBuffer.GetAddressOf()));
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to get back buffer.", false);

		// Create swap chain render target view
		hr = m_device->CreateRenderTargetView(m_backBuffer.Get(), NULL, m_swapChainRenderTargetView.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create render target view.", false);

		// Initialise final image render texture
		if (!m_finalImage.Initialise(DXGI_FORMAT_R8G8B8A8_UNORM, dimensions.x, dimensions.y))
		{
			return false;
		}

		// Initialise depth stencil
		if (!m_depthStencil.Initialise(DXGI_FORMAT_R32_FLOAT, dimensions.x, dimensions.y))
		{
			return false;
		}

		// Create default depth stencil state
		{
			CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

			hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_defaultDepthStencilState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create depth stencil state.", false);
		}

		// Create disabled depth stencil state
		{
			CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
			depthStencilDesc.DepthEnable = false;
			depthStencilDesc.StencilEnable = false;

			hr = m_device->CreateDepthStencilState(&depthStencilDesc, m_disabledDepthStencilState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create depth stencil state.", false);
		}

		// Create viewport
		m_defaultViewport = CD3D11_VIEWPORT(0.0f, 0.0f, (float)dimensions.x, (float)dimensions.y);

		// Create rasterizer state
		{
			D3D11_RASTERIZER_DESC regularRasterizerDesc;
			ZeroMemory(&regularRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

			regularRasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
			regularRasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

			hr = m_device->CreateRasterizerState(&regularRasterizerDesc, m_rasterizerState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create default rasterizer state.", false);
		}

		// Create blend state
		{
			D3D11_BLEND_DESC defaultBlendDesc;
			ZeroMemory(&defaultBlendDesc, sizeof(defaultBlendDesc));

			D3D11_RENDER_TARGET_BLEND_DESC defaultRenderTargetBlendDesc;
			ZeroMemory(&defaultRenderTargetBlendDesc, sizeof(defaultRenderTargetBlendDesc));

			defaultRenderTargetBlendDesc.BlendEnable = true;
			defaultRenderTargetBlendDesc.SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
			defaultRenderTargetBlendDesc.DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;
			defaultRenderTargetBlendDesc.BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			defaultRenderTargetBlendDesc.SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
			defaultRenderTargetBlendDesc.DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
			defaultRenderTargetBlendDesc.BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
			defaultRenderTargetBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

			defaultBlendDesc.RenderTarget[0] = defaultRenderTargetBlendDesc;

			hr = m_device->CreateBlendState(&defaultBlendDesc, m_blendState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create default blend state.", false);
		}

		// Create wrap sampler state
		{
			CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

			hr = m_device->CreateSamplerState(&samplerDesc, m_wrapSamplerState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create sampler state", false);
		}

		// Create clamp sampler state
		{
			CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

			hr = m_device->CreateSamplerState(&samplerDesc, m_clampSamplerState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create sampler state", false);
		}

        return true;
    }

	void GraphicsHandler::HandleResize(const Maths::Vec2i& dimensions)
	{
		ASSERT_RETURN(m_initialised, "GraphicsHandler needs to be initialised before use",);

		// Unbind RTV and DSV
		m_deviceContext->OMSetRenderTargets(0, 0, 0);

		// Release all outstanding references to the swap chain's buffers.
		m_swapChainRenderTargetView->Release();
		m_backBuffer->Release();

		// Release other buffers that use the same size
		m_finalImage.Release();
		m_depthStencil.Release();

		// Preserve the existing buffer count and format.
		HRESULT hr = m_swapChain->ResizeBuffers(0, dimensions.x, dimensions.y, DXGI_FORMAT_UNKNOWN, 0);
		COM_ERROR_FATAL_IF_FAILED(hr, "Window resize failed on 'IDXGISwapChain::ResizeBuffers'");

		// Get buffer and create a render-target-view.
		hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_backBuffer.GetAddressOf());
		COM_ERROR_FATAL_IF_FAILED(hr, "Window resize failed on 'IDXGISwapChain::GetBuffer'");

		hr = m_device->CreateRenderTargetView(m_backBuffer.Get(), NULL, m_swapChainRenderTargetView.GetAddressOf());
		COM_ERROR_FATAL_IF_FAILED(hr, "Window resize failed on 'ID3D11Device::CreateRenderTargetView'");

		// Set up the viewport.
		m_defaultViewport.Width = (FLOAT)dimensions.x;
		m_defaultViewport.Height = (FLOAT)dimensions.y;

		// Re-create final image render texture
		m_finalImage.Initialise(DXGI_FORMAT_R8G8B8A8_UNORM, dimensions.x, dimensions.y);

		// Re-create depth stencil
		m_depthStencil.Initialise(DXGI_FORMAT_R32_FLOAT, dimensions.x, dimensions.y);
	}

    ID3D11Device* GraphicsHandler::GetDevice() const
    {
        ASSERT(m_deviceInitialised, "GraphicsHandler's device needs to be initialised before use");

        return m_device.Get();
    }

    ID3D11DeviceContext* GraphicsHandler::GetDeviceContext() const
    {
		ASSERT(m_deviceInitialised, "GraphicsHandler's device needs to be initialised before use");

        return m_deviceContext.Get();
    }

	SpriteBatch* GraphicsHandler::GetSpriteBatch()
	{
		ASSERT(m_initialised, "GraphicsHandler needs to be initialised before use");

		return m_spriteBatch.get();
	}

	RenderTexture& GraphicsHandler::GetFinalImage()
	{
		return m_finalImage;
	}

    void GraphicsHandler::Update(double deltaTime)
    {
        ASSERT(m_initialised, "GraphicsHandler needs to be initialised before use");

		(void)deltaTime;// Remove me when deltatime is actually needed
    }

    void GraphicsHandler::Render()
    {
        ASSERT(m_initialised, "GraphicsHandler needs to be initialised before use");

		// TODO: Do fancy render stuff here
		m_deviceContext->RSSetViewports(1, &m_defaultViewport);

		m_deviceContext->RSSetState(m_rasterizerState.Get());
		m_deviceContext->OMSetDepthStencilState(m_defaultDepthStencilState.Get(), 0);

		m_deviceContext->PSSetSamplers(0, 1, m_clampSamplerState.GetAddressOf());
		m_deviceContext->OMSetBlendState(m_blendState.Get(), NULL, 0xFFFFFFFF);

		m_deviceContext->OMSetRenderTargets(1, m_finalImage.m_renderTargetView.GetAddressOf(), m_depthStencil.m_depthStencilView.Get());

		float clearColour[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_deviceContext->ClearRenderTargetView(m_finalImage.m_renderTargetView.Get(), clearColour);
		m_deviceContext->ClearDepthStencilView(m_depthStencil.m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_deviceContext->IASetInputLayout(AssetManager::Instance().GetVertexShader("$ENGINE/Shaders/Vertex/UnlitColour")->GetInputLayout());
		m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_deviceContext->VSSetShader(AssetManager::Instance().GetVertexShader("$ENGINE/Shaders/Vertex/UnlitColour")->GetShader(), NULL, 0);
		m_deviceContext->PSSetShader(AssetManager::Instance().GetPixelShader("$ENGINE/Shaders/Pixel/UnlitColour")->GetShader(), NULL, 0);

		// Render Sprite batch batches
		m_deviceContext->OMSetDepthStencilState(m_disabledDepthStencilState.Get(), 0);

		m_spriteBatch->CreateAndDrawBatches();

		// Copy final image to the back buffer
		m_deviceContext->CopyResource(m_backBuffer.Get(), m_finalImage.m_texture2D.Get());

		// Set the swap chain as the current render target for UI
		m_deviceContext->OMSetRenderTargets(1, m_swapChainRenderTargetView.GetAddressOf(), m_depthStencil.m_depthStencilView.Get());

		// ImGui Render
		Firelight::ImGuiUI::ImGuiManager::Instance()->Render();

		m_swapChain->Present(true, NULL);
    }
}
