#pragma once

#include <windows.h>
#include <comdef.h>

#include <d3d11.h>

namespace TileEngine {
  namespace D3d {

    _COM_SMARTPTR_TYPEDEF(IDXGIFactory1, __uuidof(IDXGIFactory1));
    _COM_SMARTPTR_TYPEDEF(IDXGIAdapter1, __uuidof(IDXGIAdapter1));
    _COM_SMARTPTR_TYPEDEF(IDXGISwapChain, __uuidof(IDXGISwapChain));
    _COM_SMARTPTR_TYPEDEF(ID3D11Device, __uuidof(ID3D11Device));

    _COM_SMARTPTR_TYPEDEF(ID3D11DeviceContext, __uuidof(ID3D11DeviceContext));
    _COM_SMARTPTR_TYPEDEF(ID3D11Texture2D, __uuidof(ID3D11Texture2D));
    _COM_SMARTPTR_TYPEDEF(ID3D11RenderTargetView, __uuidof(ID3D11RenderTargetView));
    _COM_SMARTPTR_TYPEDEF(ID3D11DepthStencilState, __uuidof(ID3D11DepthStencilState));
    _COM_SMARTPTR_TYPEDEF(ID3D11DepthStencilView, __uuidof(ID3D11DepthStencilView));
    _COM_SMARTPTR_TYPEDEF(ID3D11ShaderResourceView, __uuidof(ID3D11ShaderResourceView));
    _COM_SMARTPTR_TYPEDEF(ID3D11Buffer, __uuidof(ID3D11Buffer));

  } // namespace D3d
} // namespace TileEngine

