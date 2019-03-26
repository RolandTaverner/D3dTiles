#pragma once

#include <memory>

#include <boost/scoped_array.hpp>

#include <windows.h>
#include <comdef.h>

#include <d3d11.h>
#include <DirectXMath.h>

#include "RendererBase.h"

#include "D3d/DXGIFactory.h"
#include "D3d/D3dInterfaces.h"

namespace TileEngine {
  namespace D3d {

    struct TileVertex
    {
      DirectX::XMFLOAT3 Pos;
      DirectX::XMFLOAT4 Color;
    };

    class Renderer :
      public TileEngine::RendererBase
    {
    public:
      Renderer();
      virtual ~Renderer();

      void EnumerateAdapters(const AdapterReceiver &e);
      void CreateDevice(HWND hWnd, IDXGIAdapter1Ptr adapter);
      void Render();

      // RendererBase
      void RenderBitmap(unsigned level, const Rect &absRect, Bitmap::BitmapPtr s) override;
      void RenderPrimitive(unsigned level) override;


    private:
      void InitDepthStencilState();
      void InitDepthStencilBuffer(const UINT &width, const UINT &height);
      void InitDepthStencilView();

    private:
      DXGIFactory m_dxgi;
      IDXGISwapChainPtr m_swapChain;
      ID3D11DevicePtr m_device;
      ID3D11DeviceContextPtr m_deviceContext;
      ID3D11RenderTargetViewPtr m_renderTargetView;
      ID3D11Texture2DPtr m_depthStencilBuffer;
      ID3D11DepthStencilStatePtr m_depthStencilState;
      ID3D11DepthStencilViewPtr m_depthStencilView;

      DirectX::XMMATRIX m_worldMatrix;
      DirectX::XMMATRIX m_orthoMatrix;
      DirectX::XMMATRIX m_viewMatrix;

      boost::scoped_array<TileVertex> m_vertices;
    };

    typedef std::shared_ptr<Renderer> RendererPtr;

  } // namespace D3d
} // namespace TileEngine

