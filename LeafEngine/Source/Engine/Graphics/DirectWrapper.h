#ifndef DIRECT_WRAPPER_H
#define DIRECT_WRAPPER_H

#include "..\stdafx.h"

namespace Graphics
{

  class DirectWrapper
  {

  public:

    struct ConstantBufferData
    {
      DirectX::XMFLOAT4X4 worldViewProjection;
      DirectX::XMFLOAT4X4 transform;
      DirectX::XMFLOAT4 tintColor;
      DirectX::XMFLOAT2 texOffset;
      float alpha;
      float padding;
    };

    // initing, clearing, presenting frame
    void InitializeDirectX(HWND hwnd, int width, int height);
    void ClearRenderTarget();
    void Present();

    // getters for what a client might need to access (mesh.cpp for example would need to access the device)
    ID3D11Device* GetDevice() const;
    ID3D11DeviceContext* GetDeviceContext() const;
    IDXGISwapChain* GetSwapChain() const;
    ID3D11RenderTargetView* GetRenderTargetView() const;
    ID3D11VertexShader* GetVertexShader() const;
    ID3D11PixelShader* GetPixelShader() const;
    ID3DBlob* GetVertexShaderBlob() const;
    ID3DBlob* GetPixelShaderBlob() const;

    // psuedo deconstructor
    void Release();

    // Set constant buffer data
    void SetConstantBufferData(const ConstantBufferData& data);

    // Get constant buffer
    ID3D11Buffer* GetConstantBuffer() const;

    // Get input layout
    ID3D11InputLayout* GetInputLayout() const;

  private:

    // essential objects
    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;
    ID3D11VertexShader* vertexShader;
    ID3D11PixelShader* pixelShader;
    ID3DBlob* vertexShaderBlob;
    ID3DBlob* pixelShaderBlob;

    // Constant buffer
    ID3D11Buffer* constantBuffer;

    // Input layout
    ID3D11InputLayout* inputLayout;

    // internal helpers for initing
    void CreateDeviceAndSwapChain(HWND hwnd, int width, int height);
    void CreateRenderTargetView();
    void SetViewport(int width, int height);
    void LoadShaders();

  };

} // Graphics

#endif // DIRECT_WRAPPER_H
