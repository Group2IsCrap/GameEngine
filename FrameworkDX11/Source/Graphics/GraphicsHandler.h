#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace TempEngineNamespace::Graphics
{
    class GraphicsHandler
    {
    public:
        ~GraphicsHandler();

        static GraphicsHandler& Instance();

        bool Initialize(HWND hwnd, int windowWidth, int windowHeight);
        bool InitialiseDirectX(HWND hwnd, int windowWidth, int windowHeight);

        ID3D11Device*        GetDevice() const;
        ID3D11DeviceContext* GetDeviceContext() const;

        void Update(float deltaTime);
        void Render();

    private:
        GraphicsHandler();

    private:
        bool                                            mInitialised;

        Microsoft::WRL::ComPtr<ID3D11Device>            mDevice;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>     mDeviceContext;
                                                        
        Microsoft::WRL::ComPtr<IDXGISwapChain>          mSwapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  mSwapChainRenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         mBackBuffer;

        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  mDepthStencilView;
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         mDepthStencilBuffer;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState;

        CD3D11_VIEWPORT mDefaultViewport;

        Microsoft::WRL::ComPtr<ID3D11RasterizerState>   mRasterizerState;
        Microsoft::WRL::ComPtr<ID3D11BlendState>        mBlendState;
        Microsoft::WRL::ComPtr<ID3D11SamplerState>      mSamplerState;

    };
}
