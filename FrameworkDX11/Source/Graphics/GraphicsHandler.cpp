#include "GraphicsHandler.h"

#include "../Utils/ErrorManager.h"
#include "../Utils/AdapterReader.h"

namespace TempEngineNamespace::Graphics
{
    GraphicsHandler::GraphicsHandler()
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

    bool GraphicsHandler::Initialize(HWND hwnd, int windowWidth, int windowHeight)
    {
        ASSERT_RETURN(!mInitialised, "GraphicsHandler was aleady initialised", false);

        bool result = InitialiseDirectX(hwnd, windowWidth, windowHeight);
        ASSERT_RETURN(result, "DirectX initialisation failed", false);

        mInitialised = true;

        return true;
    }

    bool GraphicsHandler::InitialiseDirectX(HWND hwnd, int windowWidth, int windowHeight)
    {
		// Get graphics card adapters
		std::vector<Utils::AdapterData> adapters = Utils::AdapterReader::Instance().GetAdapters();
		ASSERT_RETURN(adapters.size() >= 1, "No DXGI Adapters Found", false);

		// Create device and swapchain
		DXGI_SWAP_CHAIN_DESC swapChainDescription;
		ZeroMemory(&swapChainDescription, sizeof(DXGI_SWAP_CHAIN_DESC));

		swapChainDescription.BufferDesc.Width = windowWidth;
		swapChainDescription.BufferDesc.Height = windowHeight;
		swapChainDescription.BufferDesc.RefreshRate.Numerator = 120; // VSync fps
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

		HRESULT hr = D3D11CreateDeviceAndSwapChain(adapters[0].mAdapter, D3D_DRIVER_TYPE_UNKNOWN,
			NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, 0, D3D11_SDK_VERSION, &swapChainDescription,
			mSwapChain.GetAddressOf(), mDevice.GetAddressOf(),
			NULL, mDeviceContext.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create device and swapchain.", false);

		// Get backbuffer
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(mBackBuffer.GetAddressOf()));
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to get back buffer.", false);

		// Create render target view
		hr = mDevice->CreateRenderTargetView(mBackBuffer.Get(), NULL, mSwapChainRenderTargetView.GetAddressOf());
		COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create render target view.", false);

		// Create depth stencil texture and view
		{
			CD3D11_TEXTURE2D_DESC depthStencilTextureDesc(DXGI_FORMAT_D24_UNORM_S8_UINT, windowWidth, windowHeight);
			depthStencilTextureDesc.MipLevels = 1;
			depthStencilTextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

			hr = mDevice->CreateTexture2D(&depthStencilTextureDesc, NULL, mDepthStencilBuffer.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create depth stencil buffer.", false);

			hr = mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), NULL, mDepthStencilView.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create depth stencil view.", false);
		}

		// Create depth stencil state
		{
			CD3D11_DEPTH_STENCIL_DESC depthStencilDesc(D3D11_DEFAULT);
			depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

			hr = mDevice->CreateDepthStencilState(&depthStencilDesc, mDepthStencilState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create depth stencil state.", false);
		}

		// Create viewport
		mDefaultViewport = CD3D11_VIEWPORT(0.0f, 0.0f, (float)windowWidth, (float)windowHeight);

		// Create rasterizer state
		{
			D3D11_RASTERIZER_DESC regularRasterizerDesc;
			ZeroMemory(&regularRasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

			regularRasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
			regularRasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;

			hr = mDevice->CreateRasterizerState(&regularRasterizerDesc, mRasterizerState.GetAddressOf());
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

			hr = mDevice->CreateBlendState(&defaultBlendDesc, mBlendState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create default blend state.", false);
		}

		// Create sampler state
		{
			CD3D11_SAMPLER_DESC samplerDesc(D3D11_DEFAULT);
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;//D3D11_FILTER_MIN_MAG_MIP_LINEAR
			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

			hr = mDevice->CreateSamplerState(&samplerDesc, mSamplerState.GetAddressOf());
			COM_ERROR_RETURN_IF_FAILED(hr, "Failed to create sampler state", false);
		}

        return true;
    }

    ID3D11Device* GraphicsHandler::GetDevice() const
    {
        ASSERT(mInitialised, "GraphicsHandler needs to be initialised before use");

        return mDevice.Get();
    }

    ID3D11DeviceContext* GraphicsHandler::GetDeviceContext() const
    {
        ASSERT(mInitialised, "GraphicsHandler needs to be initialised before use");

        return mDeviceContext.Get();
    }

    void GraphicsHandler::Update(float deltaTime)
    {
        ASSERT(mInitialised, "GraphicsHandler needs to be initialised before use");

    }

    void GraphicsHandler::Render()
    {
        ASSERT(mInitialised, "GraphicsHandler needs to be initialised before use");

		mDeviceContext->OMSetRenderTargets(1, mSwapChainRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

		float clearColour[4] = { 0.8f, 0.2f, 0.2f, 1.0f };
		mDeviceContext->ClearRenderTargetView(mSwapChainRenderTargetView.Get(), clearColour);
		mDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		// TODO: Do fancy render stuff here

		mSwapChain->Present(true, NULL);
    }
}
