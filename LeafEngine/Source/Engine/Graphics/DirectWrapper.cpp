#include "..\stdafx.h"
#include "DirectWrapper.h"
#include "..\..\Engine\LeafEngine.h"
#include <filesystem>

namespace Graphics
{

  void DirectWrapper::InitializeDirectX(HWND hwnd, int width, int height)
  {
    CreateDeviceAndSwapChain(hwnd, width, height);
    CreateRenderTargetView();
    SetViewport(width, height);
    LoadShaders();

    // Create the constant buffer
    D3D11_BUFFER_DESC constantBufferDesc = {};
    constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    constantBufferDesc.ByteWidth = sizeof(ConstantBufferData);
    constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    constantBufferDesc.CPUAccessFlags = 0;
    constantBufferDesc.MiscFlags = 0;
    device->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer);
  }

  void DirectWrapper::Release()
  {
    renderTargetView->Release();
    swapChain->Release();
    deviceContext->Release();
    device->Release();
    constantBuffer->Release();
    if (inputLayout)
    {
      inputLayout->Release();
      inputLayout = nullptr;
    }
  }

  void DirectWrapper::SetConstantBufferData(const ConstantBufferData& data)
  {
    deviceContext->UpdateSubresource(constantBuffer, 0, nullptr, &data, 0, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);
    deviceContext->PSSetConstantBuffers(0, 1, &constantBuffer);
  }

  ID3D11Buffer* DirectWrapper::GetConstantBuffer() const
  {
    return constantBuffer;
  }

  ID3D11InputLayout* DirectWrapper::GetInputLayout() const
  {
    return inputLayout;
  }

  void DirectWrapper::CreateDeviceAndSwapChain(HWND hwnd, int width, int height)
  {
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = hwnd;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    UINT flags = 0;
    // Enable debug layer
#ifdef _DEBUG = 0
    flags = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D11CreateDeviceAndSwapChain(
      nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0,
      D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
      &device, nullptr, &deviceContext
    );
  }

  void DirectWrapper::CreateRenderTargetView()
  {
    ID3D11Texture2D* backBuffer;
    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (backBuffer != 0)
    {
      device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
      backBuffer->Release();
      deviceContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
    }
  }

  void DirectWrapper::SetViewport(int width, int height)
  {
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = static_cast<FLOAT>(width);
    viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    deviceContext->RSSetViewports(1, &viewport);
  }

  void DirectWrapper::ClearRenderTarget()
  {
    const FLOAT clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
  }

  void DirectWrapper::Present()
  {
    swapChain->Present(1, 0);
  }

  void DirectWrapper::LoadShaders()
  {
    // Get the path of the current executable
    wchar_t buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    std::wstring executablePath = buffer;

    // Get the parent directory of the executable directory
    std::filesystem::path executableDir = executablePath;
    std::filesystem::path shaderDirectory = executableDir.parent_path();

    // Construct the full file paths for the shader files
    std::wstring vertexShaderPath = (shaderDirectory / L"VertexShader.cso").wstring();
    std::wstring pixelShaderPath = (shaderDirectory / L"PixelShader.cso").wstring();

    // Load the vertex shader
    HRESULT hr = D3DReadFileToBlob(vertexShaderPath.c_str(), &vertexShaderBlob);
    if (FAILED(hr))
    {
      throw std::runtime_error("Failed to load vertex shader. HRESULT: " + std::to_string(hr));
    }
    hr = device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), nullptr, &vertexShader);
    if (FAILED(hr))
    {
      throw std::runtime_error("Failed to create vertex shader. HRESULT: " + std::to_string(hr));
    }

    // Define the vertex input layout
    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
    {
      { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      { "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    // Create the input layout
    hr = device->CreateInputLayout(inputElementDesc, ARRAYSIZE(inputElementDesc), vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize(), &inputLayout);
    if (FAILED(hr))
    {
      throw std::runtime_error("Failed to create input layout. HRESULT: " + std::to_string(hr));
    }

    // Set the input layout to the device context
    deviceContext->IASetInputLayout(inputLayout);

    // Load the pixel shader
    hr = D3DReadFileToBlob(pixelShaderPath.c_str(), &pixelShaderBlob);
    if (FAILED(hr))
    {
      throw std::runtime_error("Failed to load pixel shader. HRESULT: " + std::to_string(hr));
    }
    hr = device->CreatePixelShader(pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize(), nullptr, &pixelShader);
    if (FAILED(hr))
    {
      throw std::runtime_error("Failed to create pixel shader. HRESULT: " + std::to_string(hr));
    }

    // Set the shaders to the device context
    deviceContext->VSSetShader(vertexShader, nullptr, 0);
    deviceContext->PSSetShader(pixelShader, nullptr, 0);
  }

#pragma region Getters

  ID3D11Device* DirectWrapper::GetDevice() const
  {
    return device;
  }

  ID3D11DeviceContext* DirectWrapper::GetDeviceContext() const
  {
    return deviceContext;
  }

  IDXGISwapChain* DirectWrapper::GetSwapChain() const
  {
    return swapChain;
  }

  ID3D11RenderTargetView* DirectWrapper::GetRenderTargetView() const
  {
    return renderTargetView;
  }

  ID3D11VertexShader* DirectWrapper::GetVertexShader() const
  {
    return vertexShader;
  }

  ID3D11PixelShader* DirectWrapper::GetPixelShader() const
  {
    return pixelShader;
  }

  ID3DBlob* DirectWrapper::GetVertexShaderBlob() const
  {
    return vertexShaderBlob;
  }

  ID3DBlob* DirectWrapper::GetPixelShaderBlob() const
  {
    return pixelShaderBlob;
  }

#pragma endregion End of Getters

} // Graphics
