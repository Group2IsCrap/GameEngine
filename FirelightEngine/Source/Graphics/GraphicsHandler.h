#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

#include "../Maths/Vec2.h"

#include "RenderTextures.h"

namespace Firelight::Graphics
{
    class SpriteBatch;
    class Font;
    class Text;
}

namespace Firelight::Graphics
{
    class GraphicsHandler
    {
    public:
        ~GraphicsHandler();

        static GraphicsHandler& Instance();

        bool IsInitialised() const;

        bool Initialize(HWND hwnd, const Maths::Vec2i& dimensions);
        bool InitialiseDirectX(HWND hwnd, const Maths::Vec2i& dimensions);

        void                 HandleResize(const Maths::Vec2i& dimensions);

        ID3D11Device*        GetDevice() const;
        ID3D11DeviceContext* GetDeviceContext() const;

        SpriteBatch*         GetSpriteBatch();

        RenderTexture&       GetFinalImage();

        void Update(double deltaTime);
        void Render();

    private:
        GraphicsHandler();

    private:
        bool                                            m_initialised;
        bool                                            m_deviceInitialised;

        Microsoft::WRL::ComPtr<ID3D11Device>            m_device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>     m_deviceContext;
                                                        
        Microsoft::WRL::ComPtr<IDXGISwapChain>          m_swapChain;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>  m_swapChainRenderTargetView;
        Microsoft::WRL::ComPtr<ID3D11Texture2D>         m_backBuffer;

        RenderTexture                                   m_finalImage;
        DepthRenderTexture                              m_depthStencil;

        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_defaultDepthStencilState;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_disabledDepthStencilState;

        CD3D11_VIEWPORT m_defaultViewport;

        Microsoft::WRL::ComPtr<ID3D11RasterizerState>   m_rasterizerState;
        Microsoft::WRL::ComPtr<ID3D11BlendState>        m_blendState;

        Microsoft::WRL::ComPtr<ID3D11SamplerState>      m_wrapSamplerState;
        Microsoft::WRL::ComPtr<ID3D11SamplerState>      m_clampSamplerState;

        std::unique_ptr<SpriteBatch>                    m_spriteBatch;

        Font*                                           m_testFont;
        Text*                                           m_testText;
    };
}
