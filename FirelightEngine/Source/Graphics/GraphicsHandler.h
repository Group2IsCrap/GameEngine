#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace Firelight::Graphics
{
    class GraphicsHandler
    {
    public:
        ~GraphicsHandler();

        static GraphicsHandler& Instance();

        bool Initialize(HWND hwnd, int windowWidth, int windowHeight);
        bool InitialiseDirectX(HWND hwnd, int windowWidth, int windowHeight);
        bool InitialiseImGui();

        ID3D11Device*        GetDevice() const;
        ID3D11DeviceContext* GetDeviceContext() const;

        void Update(float deltaTime);
        void Render();

    private:
        GraphicsHandler();

    private:
        bool                                            m_initialised;

        Microsoft::WRL::ComPtr<ID3D11Device>            m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_deviceContext;
                                                        
        Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_swapChainRenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_backBuffer;

        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>  m_depthStencilView;
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_depthStencilBuffer;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_depthStencilState;

        CD3D11_VIEWPORT m_defaultViewport;

        Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_rasterizerState;
        Microsoft::WRL::ComPtr<ID3D11BlendState>        m_blendState;
        Microsoft::WRL::ComPtr<ID3D11SamplerState>      m_samplerState;

    };
}
