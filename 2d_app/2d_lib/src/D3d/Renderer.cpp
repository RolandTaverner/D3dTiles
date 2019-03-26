#include "stdafx.h"

#include <string.h>

#include <directxcolors.h>

#include "D3d/Renderer.h"

namespace TileEngine {
  namespace D3d {

    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::EnumerateAdapters(const AdapterReceiver &e) {
      m_dxgi.Enumerate(e);
    }

    void Renderer::CreateDevice(HWND hWnd, IDXGIAdapter1Ptr adapter) {
      D3D_FEATURE_LEVEL featureLevels[] =
      {
          D3D_FEATURE_LEVEL_11_1,
          D3D_FEATURE_LEVEL_11_0,
          D3D_FEATURE_LEVEL_10_1,
          D3D_FEATURE_LEVEL_10_0,
      };
      UINT numFeatureLevels = ARRAYSIZE(featureLevels);

      //const D3D_FEATURE_LEVEL featureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
      //const UINT numL1evelsRequested = 1;
      D3D_FEATURE_LEVEL featureLevelsSupported;

      HRESULT hr = D3D11CreateDevice(adapter.GetInterfacePtr(),
        D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        0,
        featureLevels,
        numFeatureLevels,
        D3D11_SDK_VERSION,
        &m_device.GetInterfacePtr(),
        &featureLevelsSupported,
        &m_deviceContext.GetInterfacePtr());
      if (FAILED(hr))
      {
        throw std::runtime_error("D3D11CreateDevice() failed");
      }

      RECT rc;
      GetClientRect(hWnd, &rc);
      const UINT width = rc.right - rc.left;
      const UINT height = rc.bottom - rc.top;

      DXGI_SWAP_CHAIN_DESC sd;
      memset(&sd, 0, sizeof(sd));
      sd.BufferCount = 1;
      sd.BufferDesc.Width = width;
      sd.BufferDesc.Height = height;
      sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
      sd.BufferDesc.RefreshRate.Numerator = 60;
      sd.BufferDesc.RefreshRate.Denominator = 1;
      sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
      sd.OutputWindow = hWnd;
      sd.SampleDesc.Count = 1;
      sd.SampleDesc.Quality = 0;
      sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
      sd.Windowed = TRUE;

      m_swapChain = m_dxgi.CreateSwapChain(m_device, sd);

      m_dxgi.GetFactory()->MakeWindowAssociation(hWnd, DXGI_MWA_NO_ALT_ENTER);

      ID3D11Texture2DPtr backBuffer;
      hr = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer.GetInterfacePtr());
      if (FAILED(hr))
      {
        throw std::runtime_error("GetBuffer() failed");
      }

      hr = m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView.GetInterfacePtr());
      if (FAILED(hr))
      {
        throw std::runtime_error("CreateRenderTargetView() failed");
      }
      backBuffer.Release();

      InitDepthStencilBuffer(width, height);

      InitDepthStencilState();

      InitDepthStencilView();

      m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView.GetInterfacePtr(), m_depthStencilView);

      // Setup the viewport
      D3D11_VIEWPORT vp;
      vp.Width = (FLOAT)width;
      vp.Height = (FLOAT)height;
      vp.MinDepth = 0.0f;
      vp.MaxDepth = 1.0f;
      vp.TopLeftX = 0;
      vp.TopLeftY = 0;
      m_deviceContext->RSSetViewports(1, &vp);

      m_worldMatrix = DirectX::XMMatrixIdentity();
      m_orthoMatrix = DirectX::XMMatrixOrthographicLH(width, height, 1.0, 0.0);
    }

    void Renderer::InitDepthStencilBuffer(const UINT &width, const UINT &height)
    {
      D3D11_TEXTURE2D_DESC depthBufferDesc;
      memset(&depthBufferDesc, 0, sizeof(depthBufferDesc));

      // Set up the description of the depth buffer.
      depthBufferDesc.Width = width;
      depthBufferDesc.Height = height;
      depthBufferDesc.MipLevels = 1;
      depthBufferDesc.ArraySize = 1;
      depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      depthBufferDesc.SampleDesc.Count = 1;
      depthBufferDesc.SampleDesc.Quality = 0;
      depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
      depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
      depthBufferDesc.CPUAccessFlags = 0;
      depthBufferDesc.MiscFlags = 0;

      // Create the texture for the depth buffer using the filled out description.
      HRESULT hr = m_device->CreateTexture2D(&depthBufferDesc, nullptr, &m_depthStencilBuffer.GetInterfacePtr());
      if (FAILED(hr))
      {
        throw std::runtime_error("CreateTexture2D() failed");
      }
    }

    void Renderer::InitDepthStencilState()
    {
      D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
      memset(&depthStencilDesc, 0, sizeof(depthStencilDesc));

      depthStencilDesc.DepthEnable = false;
      depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
      depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
      depthStencilDesc.StencilEnable = true;
      depthStencilDesc.StencilReadMask = 0xFF;
      depthStencilDesc.StencilWriteMask = 0xFF;
      depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
      depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
      depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
      depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
      depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
      depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
      depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
      depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

      HRESULT hr = m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState.GetInterfacePtr());
      if (FAILED(hr))
      {
        throw std::runtime_error("CreateDepthStencilState() failed");
      }

      m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
    }

    void Renderer::InitDepthStencilView()
    {
      D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
      memset(&depthStencilViewDesc, 0, sizeof(depthStencilViewDesc));

      // Set up the depth stencil view description.
      depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
      depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
      depthStencilViewDesc.Texture2D.MipSlice = 0;

      // Create the depth stencil view.
      HRESULT hr = m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView.GetInterfacePtr());
      if (FAILED(hr))
      {
        throw std::runtime_error("CreateDepthStencilView() failed");
      }
    }

    void Renderer::Render()
    {
      // Begin scene
      // Just clear the backbuffer
      m_deviceContext->ClearRenderTargetView(m_renderTargetView, ::DirectX::Colors::Red);



      // End scene
      HRESULT hr = m_swapChain->Present(0, 0);
      if (FAILED(hr)) {
        throw std::runtime_error("Present() failed");
      }
    }

    void Renderer::RenderBitmap(unsigned level, const Rect &absRect, Bitmap::BitmapPtr s) {
    }
    
    void Renderer::RenderPrimitive(unsigned level) {
    }

  } // namespace D3d
} // namespace TileEngine
