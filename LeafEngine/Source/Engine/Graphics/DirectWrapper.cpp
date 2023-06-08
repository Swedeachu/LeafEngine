#include "DirectWrapper.h"
#include "..\..\Engine\LeafEngine.h"
#include "..\..\Engine\EngineInstance.h"
#include <filesystem>

namespace Graphics
{

	// Camera should be initialized first!
	void DirectWrapper::InitializeDirectX(HWND hwnd, int width, int height)
	{
		CreateDeviceAndSwapChain(hwnd);
		CreateRenderTargetView();
		CreateRasterizerState();
		CreateBlendStates();
		CreateConstatBuffer();
		CreateSamplers();
		SetViewPort(hwnd);
		constantBuffer.worldViewMatrix = Engine::EngineInstance.GetCamera().GetWorldMatrix();
		// Load shaders next
		LoadShaders();
		// below is setting draw settings that can be changed at any time (blend and sampler stuff for textures mainly)
		InitBlendMode();
		InitSamplerState();
	}

	template<typename ReferenceType>
	static void SmartRelease(ReferenceType& pInterface)
	{
		if (pInterface)
		{
			pInterface->Release();
		}
		pInterface = nullptr;
	}

	void DirectWrapper::ReleaseDirectX()
	{
		// Release all the blend states
		for (auto [key, value] : blendStateMap)
		{
			SmartRelease(value);
		}

		// Release all the sampler states
		for (auto [key, value] : samplerStateMap)
		{
			for (auto [key2, value2] : value)
			{
				SmartRelease(value2);
			}
		}

		// Release all our directX objects
		SmartRelease(perObjectBuffer);
		SmartRelease(inputLayout);
		SmartRelease(pixelShader);
		SmartRelease(pixelTextureShader);
		SmartRelease(vertexShader);
		SmartRelease(renderTargetView);
		SmartRelease(swapChain);
		SmartRelease(deviceContext);
		SmartRelease(device);
	}

	void DirectWrapper::CreateDeviceAndSwapChain(HWND hwnd)
	{
		// Set flags depending on if debug build or not
		D3D_FEATURE_LEVEL featureLevel;
		UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(DEBUG) || defined(_DEBUG)
		flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		// Define swap chain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.BufferCount = 2;
		swapChainDesc.OutputWindow = hwnd;
		swapChainDesc.Windowed = true;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		// apply
		D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0,
			D3D11_SDK_VERSION, &swapChainDesc, &swapChain,
			&device, &featureLevel, &deviceContext
		);
	}

	void DirectWrapper::CreateRenderTargetView()
	{
		// Create the frame buffer
		ID3D11Texture2D* frameBuffer;
		swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&frameBuffer);

		// Create render target view
		D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
		ZeroMemory(&rtDesc, sizeof(rtDesc));
		rtDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		device->CreateRenderTargetView(frameBuffer, &rtDesc, &renderTargetView);

		// we can release it now since rtv holds a reference
		frameBuffer->Release();
	}

	void DirectWrapper::CreateRasterizerState()
	{
		// Define rasterizer state
		D3D11_RASTERIZER_DESC rasterizerDesc;
		ZeroMemory(&rasterizerDesc, sizeof(rasterizerDesc));
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.DepthClipEnable = TRUE;
		rasterizerDesc.FrontCounterClockwise = FALSE;

		// Create the rasterizer state
		ID3D11RasterizerState* rasterizer;
		device->CreateRasterizerState(&rasterizerDesc, &rasterizer);

		// Set it onto the device context
		deviceContext->RSSetState(rasterizer);

		// we can release it now since we just put a ref onto the device context
		rasterizer->Release();
	}

	void DirectWrapper::CreateBlendStates()
	{
		// None Blend State
		D3D11_RENDER_TARGET_BLEND_DESC noneBlendDesc = {};
		noneBlendDesc.BlendEnable = false;
		noneBlendDesc.RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC noneBlendDescDesc = {};
		noneBlendDescDesc.AlphaToCoverageEnable = false;
		noneBlendDescDesc.RenderTarget[0] = noneBlendDesc;
		device->CreateBlendState(&noneBlendDescDesc, &blendStateMap[BlendStates::None]);

		// Transparent Blend State
		D3D11_RENDER_TARGET_BLEND_DESC transparentBlendDesc = {};
		transparentBlendDesc.BlendEnable = true;
		transparentBlendDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
		transparentBlendDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		transparentBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		transparentBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		transparentBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		transparentBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		transparentBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC transparentBlendDescDesc = {};
		transparentBlendDescDesc.AlphaToCoverageEnable = false;
		transparentBlendDescDesc.RenderTarget[0] = transparentBlendDesc;
		device->CreateBlendState(&transparentBlendDescDesc, &blendStateMap[BlendStates::Transparent]);

		// Add Blend State
		D3D11_RENDER_TARGET_BLEND_DESC addBlendDesc = {};
		addBlendDesc.BlendEnable = true;
		addBlendDesc.SrcBlend = D3D11_BLEND_ONE;
		addBlendDesc.DestBlend = D3D11_BLEND_ONE;
		addBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		addBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		addBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		addBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		addBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC addBlendDescDesc = {};
		addBlendDescDesc.AlphaToCoverageEnable = false;
		addBlendDescDesc.RenderTarget[0] = addBlendDesc;
		device->CreateBlendState(&addBlendDescDesc, &blendStateMap[BlendStates::Add]);

		// Multiply Blend State
		D3D11_RENDER_TARGET_BLEND_DESC multiplyBlendDesc = {};
		multiplyBlendDesc.BlendEnable = true;
		multiplyBlendDesc.SrcBlend = D3D11_BLEND_SRC_COLOR;
		multiplyBlendDesc.DestBlend = D3D11_BLEND_ZERO;
		multiplyBlendDesc.BlendOp = D3D11_BLEND_OP_ADD;
		multiplyBlendDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
		multiplyBlendDesc.DestBlendAlpha = D3D11_BLEND_ZERO;
		multiplyBlendDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
		multiplyBlendDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		D3D11_BLEND_DESC multiplyBlendDescDesc = {};
		multiplyBlendDescDesc.AlphaToCoverageEnable = false;
		multiplyBlendDescDesc.RenderTarget[0] = multiplyBlendDesc;
		device->CreateBlendState(&multiplyBlendDescDesc, &blendStateMap[BlendStates::Multiply]);

		// Set the default blend state
		deviceContext->OMSetBlendState(blendStateMap[BlendStates::None], nullptr, 0xffffffff);
	}

	void DirectWrapper::CreateConstatBuffer()
	{
		// Create and save constant buffer 
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(ConstantBufferData);
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		device->CreateBuffer(&bufferDesc, NULL, &perObjectBuffer);
	}

	void DirectWrapper::SetViewPort(HWND hwnd)
	{
		// get client rect of the window
		RECT rect;
		GetClientRect(hwnd, &rect);
		// Create the viewport data
		D3D11_VIEWPORT viewport = { 0.0f, 0.0f, (float)(rect.right - rect.left), (float)(rect.bottom - rect.top), 0.0f, 1.0f };
		// Set the viewport on the device context
		deviceContext->RSSetViewports(1, &viewport);
	}

	void DirectWrapper::CreateSamplers()
	{
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.MinLOD = 0;
		desc.MaxLOD = D3D11_FLOAT32_MAX;

		// Samplers with point filter 
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

		// Wrap address mode
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		device->CreateSamplerState(&desc, &samplerStateMap[SampleModes::Point][TextureAddressModes::Wrap]);

		// Clamp address mode
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		device->CreateSamplerState(&desc, &samplerStateMap[SampleModes::Point][TextureAddressModes::Clamp]);

		// Mirror address mode
		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		device->CreateSamplerState(&desc, &samplerStateMap[SampleModes::Point][TextureAddressModes::Mirror]);

		// Mirror once address mode
		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		device->CreateSamplerState(&desc, &samplerStateMap[SampleModes::Point][TextureAddressModes::Mirror_Once]);

		// Samplers with linear filter 
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

		// Wrap address mode
		desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		device->CreateSamplerState(&desc, &samplerStateMap[SampleModes::Linear][TextureAddressModes::Wrap]);

		// Clamp address mode
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		device->CreateSamplerState(&desc, &samplerStateMap[SampleModes::Linear][TextureAddressModes::Clamp]);

		// Mirror address mode
		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		device->CreateSamplerState(&desc, &samplerStateMap[SampleModes::Linear][TextureAddressModes::Mirror]);

		// Mirror once address mode
		desc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		device->CreateSamplerState(&desc, &samplerStateMap[SampleModes::Linear][TextureAddressModes::Mirror_Once]);
	}

	void DirectWrapper::BeginFrame()
	{
		float color[4] = { 0.0f, 0.0f, 0.0f, 1.0f }; // TO DO : make this not hard coded and have a private getter setter for color
		deviceContext->ClearRenderTargetView(renderTargetView, color);
		deviceContext->IASetInputLayout(inputLayout);
		deviceContext->OMSetRenderTargets(1, &renderTargetView, NULL);
	}

	// literally just presents the frame
	void DirectWrapper::EndFrame()
	{
		swapChain->Present(1, 0);
	}

	void DirectWrapper::InitBlendMode()
	{
		ID3D11BlendState* blendState = blendStateMap[BlendStates::Transparent];
		deviceContext->OMSetBlendState(blendState, NULL, 0xffffffff);
	}

	// default settings for highest quality
	void DirectWrapper::InitSamplerState()
	{
		ID3D11SamplerState** samplerState = &(samplerStateMap[SampleModes::Point][TextureAddressModes::Wrap]);
		// apply to the pixel shader
		deviceContext->PSSetSamplers(0, 1, samplerState);
	}

	void DirectWrapper::UpdateConstantBuffer()
	{
		deviceContext->UpdateSubresource(perObjectBuffer, 0, NULL, &constantBuffer, 0, 0);
		deviceContext->VSSetConstantBuffers(0, 1, &perObjectBuffer);
	}

	void DirectWrapper::SetConstantBufferTransform(const DirectX::XMFLOAT4X4* matrix)
	{
		constantBuffer.transformMatrix = *matrix;
	}

	// we shouldn't need to call this function due to our Matrx2D imeplementation in the Transform component using lazy rect to do this on the fly for us
	void DirectWrapper::SetConstantBufferTransformData(const LeafMath::Vector2D& position, const LeafMath::Vector2D& scale, float rotationRadians)
	{
		// Create the scale matrix
		DirectX::XMFLOAT4X4 scaleMatrix;
		XMStoreFloat4x4(&scaleMatrix, DirectX::XMMatrixScaling(scale.x, scale.y, 1.0f));
		// Create the rotation matrix
		DirectX::XMFLOAT4X4 rotationMatrix;
		XMStoreFloat4x4(&rotationMatrix, DirectX::XMMatrixRotationZ(rotationRadians));
		// Create the translation matrix
		DirectX::XMFLOAT4X4 translationMatrix;
		XMStoreFloat4x4(&translationMatrix, DirectX::XMMatrixTranslation(position.x, position.y, 0.0f));
		// Multiply all matrices together (composition)
		DirectX::XMFLOAT4X4 result;
		XMStoreFloat4x4(&result, XMMatrixMultiply(XMMatrixMultiply(XMLoadFloat4x4(&translationMatrix), XMLoadFloat4x4(&rotationMatrix)), XMLoadFloat4x4(&scaleMatrix)));
		// Set the transform matrix on the constant buffer
		SetConstantBufferTransform(&result);
	}

	void DirectWrapper::SetConstantBufferTextureOffset(const DirectX::XMFLOAT2& offset)
	{
		constantBuffer.texOffset = offset;
	}

	void DirectWrapper::SetConstantBufferAlpha(float alpha)
	{
		constantBuffer.alpha = alpha;
	}

	void DirectWrapper::SetConstantBufferTintColor(const DirectX::XMFLOAT4* color)
	{
		constantBuffer.tintColor = *color;
	}

	void DirectWrapper::SetWorldViewMatrix(const DirectX::XMFLOAT4X4& matrix)
	{
		constantBuffer.worldViewMatrix = matrix;
	}

	void DirectWrapper::HandleWindowResize(HWND hwnd)
	{
		// first reset the render target
		deviceContext->OMSetRenderTargets(0, 0, 0);
		// release all refs to the swap chains buffer
		renderTargetView->Release();
		// Preserve the existing buffer count and format, automatically assign window width and height
		HRESULT hr = swapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
		if (!FAILED(hr))
		{
			CreateRenderTargetView();
			SetViewPort(hwnd);
			constantBuffer.worldViewMatrix = Engine::EngineInstance.GetCamera().GetWorldMatrix();
		}
	}

	// We have it set up so our project uses visual studios hlsl compiler to build our shaders into .cso files and then at run time our binary reads them in here
	// The shaders will be in the same root path of the binary
	void DirectWrapper::LoadShaders()
	{
		// Get the path of the current executable
		wchar_t buffer[MAX_PATH];
		GetModuleFileNameW(nullptr, buffer, MAX_PATH);
		std::wstring executablePath = buffer;

		// Get the parent directory of the executable directory
		std::filesystem::path executableDir = executablePath;
		std::filesystem::path shaderDirectory = executableDir.parent_path();

		// Construct the full file paths for the shader files
		std::wstring vertexShaderPath = (shaderDirectory / L"VertexShader.cso").wstring();
		std::wstring pixelShaderPath = (shaderDirectory / L"PixelShader.cso").wstring();
		std::wstring pixelTexShaderPath = (shaderDirectory / L"PixelShaderTex.cso").wstring();

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
		D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEX", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
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

		// Load the pixel shader for textures
		hr = D3DReadFileToBlob(pixelTexShaderPath.c_str(), &pixelTexShaderBlob);
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to load pixel shader. HRESULT: " + std::to_string(hr));
		}
		hr = device->CreatePixelShader(pixelTexShaderBlob->GetBufferPointer(), pixelTexShaderBlob->GetBufferSize(), nullptr, &pixelTextureShader);
		if (FAILED(hr))
		{
			throw std::runtime_error("Failed to create pixel shader. HRESULT: " + std::to_string(hr));
		}

		// Set the shaders to the device context
		deviceContext->VSSetShader(vertexShader, nullptr, 0);
		deviceContext->PSSetShader(pixelTextureShader, nullptr, 0); // do we want texture shader to be on by default?
		deviceContext->PSSetSamplers(0, 1, &(samplerStateMap[SampleModes::Point][TextureAddressModes::Wrap])); // default was linear wrap
		// deviceContext->PSSetShader(pixelShader, nullptr, 0); 
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

#pragma endregion End of Getters

} // Graphics
